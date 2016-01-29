#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>
#include <torrentsync/dht/Node.h>
#include <boost/optional.hpp>

#include <vector>

namespace torrentsync {
namespace dht {

class NodeData;
class Node;
using namespace torrentsync;

namespace message {
namespace reply {

//! Abstract class representing every message
class FindNode : public dht::message::Message {
public:
  //! FindNode constructor to initialize the class from a raw data map
  FindNode(const torrentsync::utils::DataMap &dataMap);
  FindNode(torrentsync::utils::DataMap &&dataMap);

  FindNode(const FindNode &) = default;
  FindNode(FindNode &&) = default;

  //! Destructor
  ~FindNode() = default;

  /** creates a FindNode message reply
   * @param transactionID the ID
   * @param source source address (should be our own address)
   * @param target the target address
   * @param yield a function that returns the closest nodes to send
   *              until an invalid value is returned
   */
  template <class Container>
  static typename std::enable_if<!std::is_same<utils::Buffer, Container>::value,
                                 utils::Buffer>::type
  make(const utils::Buffer &transactionID, const dht::NodeData &source,
       Container &&container);

  template <class Container>
  static typename std::enable_if<std::is_same<utils::Buffer, Container>::value,
                                 utils::Buffer>::type
  make(const utils::Buffer &transactionID, const dht::NodeData &source,
       Container &&container);

  //! returns the parsed nodes
  std::vector<dht::NodeSPtr> getNodes() const;

  FindNode &operator=(FindNode &&) = default;
  FindNode &operator=(const FindNode &) = default;

private:
  void check() const;

  static utils::Buffer make_internal(const utils::Buffer &transactionID,
                                     const dht::NodeData &source,
                                     const utils::Buffer &nodeData);
};

bool isFindNode(const BEncodeDecoder &);

template <class Container>
typename std::enable_if<!std::is_same<utils::Buffer, Container>::value,
                        utils::Buffer>::type
FindNode::make(const utils::Buffer &transactionID, const dht::NodeData &source,
               Container &&container) {
  utils::Buffer nodeData;
  nodeData.reserve(PACKED_NODE_SIZE * DHT_FIND_NODE_COUNT);

  for (const auto &it : container)
    nodeData += it->getPackedNode();

  return make_internal(transactionID, source, nodeData);
}

template <class Container>
static typename std::enable_if<std::is_same<utils::Buffer, Container>::value,
                               utils::Buffer>::type
make(const utils::Buffer &transactionID, const dht::NodeData &source,
     Container &&container) {
  return make_internal(transactionID, source, container);
}

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */
