#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/message/reply/Ping.h>
#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/dht/message/reply/Error.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/utils/Yield.h>
#include <torrentsync/utils/log/Logger.h>

#include <exception> // for not implemented stuff

namespace torrentsync {
namespace dht {

using namespace torrentsync;
namespace msg = dht::message;

void RoutingTable::handleQuery(const dht::message::query::Ping &ping,
                               const dht::Node &node) {
  assert(!!(node.getEndpoint()));

  // send ping reply
  sendMessage(
      msg::reply::Ping::make(ping.getTransactionID(), _table.getTableNode()),
      *(node.getEndpoint()));
}

//! Handle ping reply.
void RoutingTable::handleReply(const dht::message::reply::Ping &message,
                               const dht::Node &node) {
  LOG(DEBUG, "Ping Reply received " << message.getID() << " " << node);
}

void RoutingTable::handleQuery(const dht::message::query::FindNode &message,
                               const dht::Node &node) {
  LOG(DEBUG, "Find Query received " << message.getID() << " " << node);
  assert(!!(node.getEndpoint()));

  std::lock_guard<std::mutex> lock_table(_table_mutex);
  auto nodes = _table.getClosestNodes(node);
  sendMessage(msg::reply::FindNode::make(
                  message.getTransactionID(), _table.getTableNode(),
                  utils::makeYield<dht::NodeSPtr>(nodes.cbegin(), nodes.cend())
                      .function()),
              *(node.getEndpoint()));
}

void RoutingTable::handleReply(const dht::message::reply::FindNode &message,
                               const dht::Node &node) {
  LOG(WARN, "find_node reply without a callback. " << message.getID() << "-"
                                                   << node);
}

void RoutingTable::doPing(dht::Node &destination) {
  assert(!!(destination.getEndpoint()));

  torrentsync::utils::Buffer transaction = newTransaction();

  torrentsync::utils::Buffer ping =
      msg::query::Ping::make(transaction, _table.getTableNode());

  registerCallback(
      [&](boost::optional<Callback::payload_type> data,
          const torrentsync::dht::Callback & /* trigger */) {

        if (!!data) {
          const auto *reply = boost::get<msg::reply::Reply>(&data->message);
          assert(reply != nullptr);
          const auto *error = boost::get<msg::reply::Error>(reply);
          if (error != nullptr) {
            LOG(DEBUG, "Error in ping. " << *error);
          } else {
            data->node.setGood(); // mark the node as good
            LOG(DEBUG, "Ping handled: " << data->node);
          }
        }
      },
      transaction, destination);

  // send ping reply
  sendMessage(ping, *(destination.getEndpoint()));
}

void RoutingTable::sendError(const udp::endpoint &ip,
                             const utils::Buffer &transaction,
                             const dht::message::ErrorType::error_type err) {
  utils::Buffer error = msg::reply::Error::make(transaction, err);
  sendMessage(error, ip);
}

} // dht
} // torrentsync
