#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/utils/log/Logger.h>

#include <exception> // for not implemented stuff

namespace torrentsync {
namespace dht {

using namespace torrentsync;
namespace msg = dht::message;

void RoutingTable::recvMessage(const boost::system::error_code &error,
                               utils::Buffer buffer,
                               std::size_t bytes_transferred,
                               const boost::asio::ip::udp::endpoint &sender) {
  namespace msg = dht::message;

  buffer.resize(bytes_transferred);
  LOG(DEBUG, "Routingtable * received a message from "
                 << sender << " e:" << error.message());
  LOG(DATA, "RoutingTable * " << sender << " received " << bytes_transferred
                              << " " << pretty_print(buffer));

  // check for errors
  if (error) {
    LOG(ERROR, "RoutingTable * recvMessage error: " << error << " "
                                                    << error.message());
    return;
  }

  // parse the message

  try {
    msg::AnyMessage message = msg::parseMessage(buffer);
    LOG(DATA, "RoutingTable * message parsed: " << msg::getString(message));
    processIncomingMessage(message, buffer, sender);
  } catch (const msg::MessageException &e) {
    LOG(ERROR, "RoutingTable * message parsing failed" << e.what());
    LOG(DATA,
        "RoutingTable * message parsing failed: " << pretty_print(buffer));
    // @TODO send malformed message error
  }
}

void RoutingTable::processIncomingMessage(
    torrentsync::dht::message::AnyMessage &message, const utils::Buffer &buffer,
    const boost::asio::ip::udp::endpoint &sender) {
  const auto type = msg::getType(message);

  // fetch the node from the tree table
  boost::optional<NodeSPtr> node;
  {
    std::lock_guard<std::mutex> lock_table(_table_mutex);
    node = _table.getNode(NodeData(msg::getID(message)));
  }

  if (!!node) // we already know the node
  {
    const auto endpoint = (*node)->getEndpoint();
    // message dropped if t he data is still fresh but with a different IP.
    if (!!endpoint && *endpoint != sender)
      return;
  } else {
    // create new node
    const utils::Buffer id = msg::getID(message);
    LOG(DEBUG, "New node: " << pretty_print(id) << " addr: " << sender);
    node = boost::optional<NodeSPtr>(NodeSPtr(new Node(id, sender)));
  }

  // if a callback is registered call it instead of the normal flow
  auto callback = getCallback(message);
  if (!!callback) {
    callback->call(message, **node);
  } else {
    LOG(DEBUG, "Callback not found");
    const auto *query = boost::get<msg::query::Query>(&message);
    if (query != nullptr) {
      try {
        const auto *ping = boost::get<msg::query::Ping>(query);
        if (ping != nullptr) {
          handleQuery(*ping, **node);
        }

        const auto *find_node = boost::get<msg::query::FindNode>(query);
        if (find_node != nullptr) {
          handleQuery(*find_node, **node);
        } else {
          LOG(ERROR, "RoutingTable * unknown query type: "
                         << pretty_print(buffer) << " - "
                         << msg::getString(message));
        }
      } catch (const dht::message::MessageException &e) {
        LOG(ERROR,
            " RoutingTable * malformed message: " << msg::getString(message));
      }
    } else if (type == msg::Type::Reply) {
      // Replies should be all managed by a Callback
      // Log the message and drop it, it's an unexpected reply.
      // Maybe it's a reply that came to late or what not.
      LOG(INFO, "RoutingTable * received unexpected reply: "
                    << msg::getString(message) << " " << sender);
    } else if (type == msg::Type::Error) {
      // same be behaviour as a Reply without a callback
      LOG(WARN, "RoutingTable * received unexpected error: "
                    << msg::getString(message) << " " << sender);
    } else {
      LOG(ERROR, "RoutingTable * unknown message type: "
                     << pretty_print(buffer) << " - "
                     << msg::getString(message));
    }
  }

  // @TODO post-process
  // - add the node to the known addresses
  // - update node statistics

  // - add the node to the tree in case it's missing, or set it good
  // - update transaction id if it was a query
}

} // dht
} // torrentsync
