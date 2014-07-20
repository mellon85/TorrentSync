#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/utils/Finally.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/Ping.h>

#include <iterator>
#include <vector>
#include <atomic>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/assign/list_of.hpp>

//! list of known bootstrap servers for the DHT network
static const std::list< // domain/ip address, port, needs to be resolved
    boost::tuple<std::string,unsigned short,bool> > BOOTSTRAP_ADDRESSES =
        boost::assign::tuple_list_of
            ("router.bittorrent.com",6881,true)
            ("router.utorrent.com"  ,6881,true);

//! 3 batches per second while initializing the DHT, should be configurable.
static const size_t INITIALIZE_PING_BATCH_INTERVAL = static_cast<size_t>(1000/3);

//! Maximum amount of packets in the send queue.
//! In case there are more then the package will be dropped.
static const size_t MAX_SEND_QUEUE = 100;

namespace torrentsync
{
namespace dht
{

RoutingTable::RoutingTable(
    boost::asio::io_service& io_service)
        : _table(NodeData::getRandom()),
          _io_service(io_service),
          _recv_socket(io_service),
          _send_socket(io_service),
          _close_nodes_count(0)
{
    LOG(INFO, "RoutingTable * Peer Node: " << _table.getPeerNode());
}

RoutingTable::~RoutingTable()
{
}

udp::endpoint RoutingTable::getEndpoint() const
{
    return _recv_socket.local_endpoint();
}

void RoutingTable::initializeTable( shared_timer timer )
{
    if (!timer)
        timer = shared_timer(new boost::asio::deadline_timer(_io_service,
            boost::posix_time::milliseconds(INITIALIZE_PING_BATCH_INTERVAL)));

    LOG(DEBUG, "RoutingTable * Register initializeTable timer");
    timer->async_wait(
        [this,timer] (const boost::system::error_code& e) {
                using namespace boost::asio;

                if ( e.value() != 0 )
                {
                    LOG(ERROR, "Error in RoutingTable initializeTable timer: " << e.message());
                    initializeTable(timer);
                }

                if ( _initial_addresses.empty() )
                    bootstrap();

                for( size_t i = 0; 
                    i < INITIALIZE_PING_BATCH_SIZE && !_initial_addresses.empty(); ++i )
                {   
                    // copy to local and delete head
                    const auto endpoint = _initial_addresses.front();
                    _initial_addresses.pop_front();

                    LOG(DEBUG, "RoutingTable * initializing ping with " << endpoint);

                    // create and send the message
                    torrentsync::utils::Buffer msg =
                        torrentsync::dht::message::Ping::getMessage(
                            torrentsync::utils::Buffer("aab"),
                            _table.getPeerNode());

                    // send the message
                    sendMessage(msg,endpoint);

                    // TODO send find_node to get our node
                    // give the 8 nodes priority over others. once we start having
                    // enough fresh nodes we don't need additional old nodes and we
                    // can skip them.

                    LOG(ERROR, "RoutingTable * initializeTable lambda not implemented yet!");
                }

                LOG(DEBUG, "RoutingTable * " << _initial_addresses.size() <<
                                " initializing addresses remaining");

                if ( _close_nodes_count < DHT_CLOSE_ENOUGH )
                    initializeTable(timer);
            });
}

void RoutingTable::bootstrap()
{
    if (!_initial_addresses.empty())
    {
        LOG(INFO,"RoutingTable * bootstrap Can't bootstrap while I still " <<
                "have initial addresses");
        return;
    }
    

    LOG(INFO,"RoutingTable * Proceeding with boostrap procedure from " <<
            "known nodes: " << _table.size() << "-" << _close_nodes_count);

    //! TODO proceed with bootstrap
    throw std::runtime_error("bootstrap Not Implemented Yet");
}

void RoutingTable::tableMaintenance()
{
    throw std::runtime_error("Not Implemented Yet");
}

void RoutingTable::initializeNetwork(
    const udp::endpoint& endpoint )
{
    if (_send_socket.is_open() || _recv_socket.is_open())
        throw std::runtime_error("The Routing table network has already been initialized");
    LOG(INFO, "RoutingTable * Bind Node: " << endpoint);
    _send_socket.open(endpoint.protocol());
    _recv_socket.open(endpoint.protocol());
    _recv_socket.bind(endpoint);
    scheduleNextReceive();
    initializeTable();
}

void RoutingTable::scheduleNextReceive()
{
    torrentsync::utils::Buffer buff(MESSAGE_BUFFER_SIZE);
    boost::shared_ptr<boost::asio::ip::udp::endpoint> sender(
        new boost::asio::ip::udp::endpoint());

    _recv_socket.async_receive_from(
            boost::asio::mutable_buffers_1(buff.begin(),buff.size()),
            *sender,
            [=] ( const boost::system::error_code& error,
                  std::size_t bytes_transferred) -> void 
                { recvMessage(error,buff,bytes_transferred,*sender); });
}

void RoutingTable::registerCallback(
    const Callback::callback& func,
    const std::string& type,
    const std::string& messageType,
    const torrentsync::dht::NodeData& source, 
    const boost::optional<torrentsync::utils::Buffer>& transactionID)
{
    _callbacks.insert(
        std::make_pair(
            source,
            Callback(func,type,messageType,source,transactionID)));
}

boost::optional<Callback> RoutingTable::getCallback(
    const message::Message& message)
{
    auto its = _callbacks.equal_range(message.getID());

    boost::optional<Callback> ret;
    for( auto it = its.first; it != its.second; ++it )
    {
        if (it->second.verifyConstraints(message))
        {
            ret = it->second;
            _callbacks.erase(it);
            break;
        }
    }

    return ret;
}

void RoutingTable::sendMessage(
    const torrentsync::utils::Buffer& buff,
    const udp::endpoint& addr)
{
    static std::atomic<size_t> send_queue_counter(0);

    // the write handler will ensure that the buffer exists until the
    // end of the send.
    buff.freeze();
    WriteLock lock(_send_mutex);

    const size_t count = send_queue_counter.fetch_add(1);
    if (count != MAX_SEND_QUEUE)
    {
        _recv_socket.async_send_to(
            boost::asio::buffer(buff.get(),buff.size()),
            addr,
            [=] (
                  const boost::system::error_code& error,
                  std::size_t bytes_transferred) -> void 
                { send_queue_counter--;
                  LOG(DEBUG,"RoutingTable * Sent to " << addr << " " <<
                    bytes_transferred << "/" << buff.size() << " e:" <<
                    error.message() << " buffer:" << pretty_print(buff)); });
    } else {
        LOG(DEBUG,"RoutingTable * dropped to " << addr << " " << " buffer:"<<buff);
        send_queue_counter--;
    }
}

void RoutingTable::recvMessage(
    const boost::system::error_code& error,
    torrentsync::utils::Buffer buffer,
    std::size_t bytes_transferred,
    const boost::asio::ip::udp::endpoint& sender)
{
    torrentsync::utils::Finally([&](){scheduleNextReceive();});

    namespace msg = torrentsync::dht::message;

    // safely terminate the buffer
    buffer[bytes_transferred] = 0;
    buffer.freeze();

    LOG(DEBUG,"RoutingTable * from "<< sender << " received " << bytes_transferred <<  " " << pretty_print(buffer) << " e:"<<error.message());
    boost::shared_ptr<msg::Message> message;

    // check for errors
    if (error)
    {
        LOG(ERROR, "RoutingTable * recvMessage error: " << error << " " << error.message());
        return;
    }

    // parse the message
    try
    {
         message = msg::Message::parseMessage(buffer,bytes_transferred);
    }
    catch ( const msg::BEncodeException& e )
    {
        LOG(ERROR, "RoutingTable * message parsing failed: " << pretty_print(buffer) << " e:" << e.what());
        return;
    }

    // @TODO execute pre-process pass
    // @TODO process the message
    // @TODO execute post-process pass

    throw std::runtime_error("Not Implemented Yet");
}

boost::shared_ptr<boost::asio::ip::tcp::socket> RoutingTable::lookForNode()
{
    throw std::runtime_error("Not Implemented Yet");
    boost::shared_ptr<boost::asio::ip::tcp::socket> ret;
    return ret;
}

}; // dht
}; // torrentsync

