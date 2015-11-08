#include <torrentsync/utils/Buffer.h>
#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Finally.h>
#include <torrentsync/dht/RoutingTable.h>

#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/message/Constants.h>

#include <iterator>
#include <vector>
#include <tuple>
#include <memory>

#include <boost/asio.hpp>
#include <boost/cast.hpp>

//! Maximum amount of packets in the send queue.
//! In case there are more then the package will be dropped.
static const size_t MAX_SEND_QUEUE = 100;

//! Maximum number of seconds to wait for a reply.
static const size_t ROUTINGTABLE_TIMEOUT = 10;

namespace torrentsync {
namespace dht {

namespace msg = dht::message;

RoutingTable::RoutingTable(boost::asio::io_service &io_service)
    : _initialization_completed(false), _table(NodeData::getRandom()),
      _io_service(io_service), _recv_socket(io_service),
      _send_socket(io_service), _send_queue_counter(0),
      _transaction_id(rand()) // initialize with a random value
{
  LOG(INFO, "RoutingTable * Table Node: " << _table.getTableNode());
}

udp::endpoint RoutingTable::getEndpoint() const {
  return _recv_socket.local_endpoint();
}

void RoutingTable::tableMaintenance() {

  const auto timer = std::make_shared<boost::asio::deadline_timer>(
      _io_service, boost::posix_time::minutes(5));

  timer->async_wait([&, timer](const boost::system::error_code &e) {
    LOG(DEBUG, "table maintenance running");
    tableMaintenance();
    static std::mutex running;

    if (e.value() != 0) {
      LOG(ERROR,
          "Error in RoutingTable tableMaintenance timer: " << e.message());
      return;
    }

    if (!running.try_lock()) {
      LOG(DEBUG, "RoutingTable * Initializations till running");
      return;
    }

    std::lock_guard<std::mutex> lock(_table_mutex);
    throw std::runtime_error("Not Implemented Yet");

    bool good_nodes_known = false;

    // @TODO
    // - clean the buckets. set good_nodes_known when a good node is found
    // - start bucket refilling.
    //   if a bucket is low, start a procedure to look for a random
    //   address in the bucket. Asking one of the element of the buckert,
    //   or any other node untl the bucket is full again.
    // start initialization procedure in case all known nodes disappear.
    if (!good_nodes_known) {
      initializeTable();
    }
  });
}

void RoutingTable::initializeNetwork(const udp::endpoint &endpoint) {
  if (_send_socket.is_open() || _recv_socket.is_open())
    throw std::runtime_error(
        "The Routing table network has already been initialized");
  LOG(INFO, "RoutingTable * Bind Node: " << endpoint);
  _send_socket.open(endpoint.protocol());
  _recv_socket.open(endpoint.protocol());
  _recv_socket.bind(endpoint);
  scheduleNextReceive();
  initializeTable();
}

void RoutingTable::scheduleNextReceive() {
  auto buff = std::make_shared<utils::Buffer>();
  buff->assign(MESSAGE_BUFFER_SIZE, 0); // assign buffer size
  auto sender = std::make_shared<boost::asio::ip::udp::endpoint>();

  LOG(DEBUG, "Scheduling receive");

  _recv_socket.async_receive_from(
      boost::asio::buffer(*buff, MESSAGE_BUFFER_SIZE), *sender,
      [&, sender, buff](const boost::system::error_code &error,
                        std::size_t bytes_transferred) -> void {
        utils::Finally([&]() { scheduleNextReceive(); });
        recvMessage(error, *buff, bytes_transferred, *sender);
      });
}

void RoutingTable::registerCallback(
    const Callback::callback_t &func, const utils::Buffer &transactionID,
    const boost::optional<dht::NodeData> &source) {
  LOG(DEBUG, "Adding callback for transation " << pretty_print(transactionID));
  _callbacks.insert(
      std::make_pair(transactionID, Callback(func, source, transactionID)));
}

boost::optional<Callback>
RoutingTable::getCallback(const message::AnyMessage &message) {
  auto its = _callbacks.equal_range(msg::getTransactionID(message));

  boost::optional<Callback> ret;
  for (auto it = its.first; it != its.second; ++it) {
    if (it->second.verifyConstraints(message)) {
      ret = it->second;
      _callbacks.erase(it);
      break;
    }
  }

  return ret;
}

const NodeData &RoutingTable::getNode() const noexcept {
  return _table.getTableNode();
}

void RoutingTable::sendMessage(const utils::Buffer &buff,
                               const udp::endpoint &addr) {
  // the write handler will ensure that the buffer exists until the
  // end of the send.

  const size_t count =
      _send_queue_counter.fetch_add(1, std::memory_order_relaxed);
  if (count < MAX_SEND_QUEUE) {
    _recv_socket.async_send_to(
        boost::asio::buffer(buff), addr,
        [=](const boost::system::error_code &error,
            std::size_t bytes_transferred) -> void {
          _send_queue_counter.fetch_sub(1, std::memory_order_relaxed);
          LOG(DEBUG, "RoutingTable * Sent to "
                         << addr << " " << bytes_transferred << "/"
                         << buff.size() << " e:" << error.message()
                         << " buffer:" << pretty_print(buff));
        });
  } else {
    LOG(DEBUG, "RoutingTable * dropped to " << addr << " "
                                            << " buffer:" << buff);
    _send_queue_counter.fetch_sub(1, std::memory_order_relaxed);
  }
}

std::shared_ptr<boost::asio::ip::tcp::socket> RoutingTable::lookForNode() {
  throw std::runtime_error("Not Implemented Yet");
  std::shared_ptr<boost::asio::ip::tcp::socket> ret;
  return ret;
}

utils::Buffer RoutingTable::newTransaction() {
  utils::Buffer buff;
  buff.reserve(2);

  // as per BEP 005 suggestion
  uint16_t value = _transaction_id.fetch_add(1, std::memory_order_relaxed);

  buff.push_back(value);
  buff.push_back(value >> 8);
  return buff;
}

}; // dht
}; // torrentsync
