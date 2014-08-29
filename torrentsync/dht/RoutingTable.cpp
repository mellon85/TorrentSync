#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/utils/Finally.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/Ping.h>
#include <torrentsync/dht/message/FindNode.h>

#include <iterator>
#include <vector>
#include <atomic>
#include <mutex>
#include <tuple>

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/cast.hpp>

//! list of known bootstrap servers for the DHT network
static std::vector< // domain/ip address, port, needs to be resolved
    std::pair<std::string,std::string> > BOOTSTRAP_ADDRESSES =
            {std::make_pair("router.bittorrent.com"     ,"8991"),
             std::make_pair("router.utorrent.com"       ,"6881"),
             std::make_pair("dht.transmissionbt.com"    ,"6881")
    };

//! number of batches per second while initializing the DHT.
static const size_t INITIALIZE_PING_BATCH_INTERVAL = static_cast<size_t>(3000);

//! Maximum amount of packets in the send queue.
//! In case there are more then the package will be dropped.
static const size_t MAX_SEND_QUEUE = 100;

//! Number of nodes that return my address when looking for my node
//! to be sure that I can't get any closer.
static const size_t DHT_CLOSE_ENOUGH = 10;

//! Maximum number of seconds to wait for a reply.
static const size_t ROUTINGTABLE_TIMEOUT = 10;


namespace torrentsync
{
namespace dht
{

// utorrent bootstrap message
// d2:ip6:_0xaaR0xee0x940xb11:rd2:id20:.0x1d0xe6isQ0xffJ0xec)0xcd0xba0xab0xf20xfb0xe3F|0xc2ge1:t4:0x00x0jE1:y1:re

namespace msg = torrentsync::dht::message;

RoutingTable::RoutingTable(
    boost::asio::io_service& io_service)
        : _table(NodeData::getRandom()),
          _io_service(io_service),
          _recv_socket(io_service),
          _send_socket(io_service),
          _close_nodes_count(0),
          _transaction_id(rand())
{
    LOG(INFO, "RoutingTable * Table Node: " << _table.getTableNode());
}

RoutingTable::~RoutingTable()
{
}

udp::endpoint RoutingTable::getEndpoint() const
{
    return _recv_socket.local_endpoint();
}

void RoutingTable::initializeTable()
{
    using timer_t = boost::asio::deadline_timer;
     std::shared_ptr<timer_t> timer(new timer_t(_io_service,
        boost::posix_time::milliseconds(INITIALIZE_PING_BATCH_INTERVAL)));

    LOG(DEBUG, "RoutingTable * Register initializeTable timer");
    timer->async_wait(
        [&,timer] (const boost::system::error_code& e) {
                using namespace boost::asio;

                LOG(DEBUG, "RoutingTable * " << _initial_addresses.size() <<
                                " initializing addresses");

                if ( e.value() != 0 )
                {
                    LOG(ERROR, "Error in RoutingTable initializeTable timer: " << e.message());
                    initializeTable();
                    return;
                }

                if ( _initial_addresses.empty() )
                {
                    bootstrap();
                    LOG(DEBUG, "RoutingTable * " << _initial_addresses.size() <<
                                    " initializing addresses after bootstrap");
                }

                for( size_t i = 0; 
                    i < INITIALIZE_PING_BATCH_SIZE && !_initial_addresses.empty(); ++i )
                {   
                    // copy to local and delete head
                    const auto endpoint = _initial_addresses.front();
                    _initial_addresses.pop_front();

                    LOG(DEBUG, "RoutingTable * initializing ping with " << endpoint);
/*
                    torrentsync::utils::Buffer msg = msg::Ping::getQuery(
                        newTransaction(),
                        _table.getTableNode());
*/
                    // create and send the message
                    torrentsync::utils::Buffer msg =
                        torrentsync::dht::message::FindNode::getMessage(
                            newTransaction(),
                            _table.getTableNode(),
                            _table.getTableNode());

                    
                    // send the message
                    sendMessage(msg,endpoint);

                    // @TODO send find_node to get our node
                    // give the 8 nodes priority over others. once we start having
                    // enough fresh nodes we don't need additional old nodes and we
                    // can skip them.
                }

                LOG(DEBUG, "RoutingTable * " << _initial_addresses.size() <<
                                " initializing addresses remaining");

                if ( _close_nodes_count < DHT_CLOSE_ENOUGH )
                    initializeTable();
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
            "known nodes; count: " << _table.size() << ", needed: " << _close_nodes_count);

    udp::resolver resolver(_io_service);

    std::for_each( BOOTSTRAP_ADDRESSES.begin(), BOOTSTRAP_ADDRESSES.end(),
        [&](const std::pair<std::string,std::string>& addr)
    {
        LOG(DEBUG,"Bootstrapping with "<<addr.first<<":"<<addr.second);
        boost::system::error_code error;
        udp::resolver::query query(udp::v4(), addr.first, addr.second);
        udp::resolver::iterator iterator = resolver.resolve(query,error);

        while ( iterator != udp::resolver::iterator() )
        {
            LOG(DEBUG,"Resolved to "<< iterator->host_name() );
            _initial_addresses.push_back(iterator->endpoint());
            ++iterator;
        }
    });
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
    std::shared_ptr<utils::Buffer> buff(
        new utils::Buffer);
    buff->reserve(MESSAGE_BUFFER_SIZE);
    buff->assign(4096,0);
    std::shared_ptr<boost::asio::ip::udp::endpoint> sender(
        new boost::asio::ip::udp::endpoint());

    LOG(DEBUG,"Scheduling receive"); 
    
    _recv_socket.async_receive_from(
            boost::asio::buffer(*buff,MESSAGE_BUFFER_SIZE),
            *sender,
            [&,sender,buff] (
                  const boost::system::error_code& error,
                  std::size_t bytes_transferred) -> void 
                {
                utils::Finally([&](){scheduleNextReceive();});
                recvMessage(error,*buff,bytes_transferred,*sender);
            });
}

void RoutingTable::registerCallback(
    const Callback::callback_t& func,
    const torrentsync::dht::NodeData& source, 
    const torrentsync::utils::Buffer& transactionID)
{
    _callbacks.insert(
        std::make_pair(
            source,
            Callback(func,source,transactionID)));
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
    std::lock_guard<std::mutex> lock(_send_mutex);

    const size_t count = send_queue_counter.fetch_add(1);
    if (count < MAX_SEND_QUEUE)
    {
        _recv_socket.async_send_to(
            boost::asio::buffer(buff),
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
    utils::Buffer buffer,
    std::size_t bytes_transferred,
    const boost::asio::ip::udp::endpoint& sender)
{
    namespace msg = torrentsync::dht::message;
    
    buffer.resize(bytes_transferred);
    LOG(DEBUG,"RoutingTable * from " << sender << " received " <<
        bytes_transferred <<  " " << pretty_print(buffer)
        << " e:" << error.message());

    std::shared_ptr<msg::Message> message;

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
        LOG(DEBUG, "RoutingTable * message parsed: \n" << *message);
    }
    catch ( const msg::MalformedMessageException& e )
    {
        LOG(ERROR, "RoutingTable * message parsing failed: " << pretty_print(buffer) << " e:" << e.what());
        return;
    }

    const auto msg_type = message->getMessageType();
    const auto type     = message->getType();

    // fetch the node from the tree table
    boost::optional<NodeSPtr> node = _table.getNode( message->getID() );
    
    if (!!node) // we already know the node
    {
        const auto endpoint = (*node)->getEndpoint();
        // message dropped if the data is still fresh but with a different IP.
        if (!!endpoint && *endpoint != sender)
            return;
    }
    else
    {
        // create new node
        node = boost::optional<NodeSPtr>(NodeSPtr(
            new Node(message->getID(),sender)));
    }

    // if a callback is registered call it instead of the normal flow
    auto callback = getCallback(*message);
    if( !!callback ) 
    {
        callback->call(*message,**node);
    }
    else
    {
        if ( type == msg::Type::Query )
        {
            assert( !!msg_type );
            try
            {
                if ( *msg_type == msg::Messages::Ping )
                {
                    handlePingQuery(
                        *boost::polymorphic_downcast<msg::Ping*>(message.get()),
                        **node);
                }
                else if ( *msg_type == msg::Messages::FindNode )
                {
                    handleFindNodeQuery(
                        *boost::polymorphic_downcast<msg::FindNode*>(message.get()),
                        **node);
                } 
                else
                {
                   LOG(ERROR, "RoutingTable * unknown query type: " << pretty_print(buffer) << " - " << message);
                }
            }
            catch ( std::bad_cast& e ) 
            {
                LOG(ERROR, " RoutingTable * A message was mis-interpreted! " << message << " Report this bug! ");
            }
        }
        else if (type == msg::Type::Reply)
        {
            assert( !msg_type );
            // Replies should be all managed by a Callback
            // Log the message and drop it, it's an unexpected reply.
            // Maybe it's a reply that came to late or what not.
            LOG(INFO, "RoutingTable * received unexpected reply: \n" << *message << " " << sender);
        }
        else if ( type == msg::Type::Error )
        {
            // @TODO implement error handling
        }
        else 
        {
            LOG(ERROR, "RoutingTable * unknown message type: " << pretty_print(buffer) << " - " << message);
        }
    }
 
    // @TODO post-process
    // - add the node to the known addresses
    // - update node statistics

    // - add the node to the tree in case it's missing, or set it good
    // - update transaction id if it was a query
}

std::shared_ptr<boost::asio::ip::tcp::socket> RoutingTable::lookForNode()
{
    throw std::runtime_error("Not Implemented Yet");
    std::shared_ptr<boost::asio::ip::tcp::socket> ret;
    return ret;
}

utils::Buffer RoutingTable::newTransaction()
{
    utils::Buffer buff;
    buff.reserve(2);
    uint16_t value = _transaction_id++; // as per BEP 005 suggestion
    buff.push_back(value);
    buff.push_back(value>>8);
    return buff;
}

}; // dht
}; // torrentsync

