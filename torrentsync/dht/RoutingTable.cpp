#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/utils/Finally.h>
#include <torrentsync/dht/RoutingTable.h>

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/Constants.h>

#include <iterator>
#include <vector>
#include <atomic>
#include <mutex>
#include <tuple>

#include <boost/asio.hpp>
#include <boost/cast.hpp>

//! Maximum amount of packets in the send queue.
//! In case there are more then the package will be dropped.
static const size_t MAX_SEND_QUEUE = 100;

//! Maximum number of seconds to wait for a reply.
static const size_t ROUTINGTABLE_TIMEOUT = 10;

namespace torrentsync
{
namespace dht
{

namespace msg = dht::message;

RoutingTable::RoutingTable(
    boost::asio::io_service& io_service)
        : _table(NodeData::getRandom()),
          _io_service(io_service),
          _recv_socket(io_service),
          _send_socket(io_service),
          _close_nodes_count(0),
          _transaction_id(rand()) // initialize with a random value
{
    LOG(INFO, "RoutingTable * Table Node: " << _table.getTableNode());
}

udp::endpoint RoutingTable::getEndpoint() const
{
    return _recv_socket.local_endpoint();
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
    buff->assign(MESSAGE_BUFFER_SIZE,0); // assign buffer size
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
    const utils::Buffer& transactionID,
    const boost::optional<dht::NodeData>& source)
{
    _callbacks.insert(
        std::make_pair(
            transactionID,
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
    const utils::Buffer& buff,
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

void RoutingTable::sendError(
        udp::endpoint& ip,
        dht::message::ErrorType::error_type err)
{
    //! @TODO send error message
}

}; // dht
}; // torrentsync
