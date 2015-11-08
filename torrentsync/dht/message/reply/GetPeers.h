#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/Node.h>

#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/DHTConstants.h>

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
class GetPeers : public dht::message::Message {
public:
  //! GetPeers constructor to initialize the class from a raw data map
  GetPeers(const DataMap &dataMap);

  GetPeers(GetPeers &&) = default;
  GetPeers(const GetPeers &) = default;

  //! Destructor
  ~GetPeers() = default;

  /** creates a GetPeers message reply
   * @param transactionID the ID
   * @param token the authentication token from previous messages
   * @param source source address (should be our own address)
   * @param target the target address
   * @param elements a container with either NodeSPtr or udp::enpoints to reply
   *         with.
   * @throws MessageException in case the yield object doesn't contain
   *         items.
   */
  template <class Container>
  static typename std::enable_if<
      std::is_same<typename std::remove_reference<Container>::type::value_type,
                   NodeSPtr>::value,
      utils::Buffer>::type
  make(const utils::Buffer &transactionID, const utils::Buffer &token,
       const dht::NodeData &source, Container &&elements);

  template <class Container>
  static typename std::enable_if<
      std::is_same<typename std::remove_reference<Container>::type::value_type,
                   udp::endpoint>::value,
      utils::Buffer>::type
  make(const utils::Buffer &transactionID, const utils::Buffer &token,
       const dht::NodeData &source, Container &&elements);

  //! returns the parsed nodes
  boost::optional<std::vector<dht::NodeSPtr>> getNodes() const;

  //! return endpoints
  boost::optional<std::list<udp::endpoint>> getPeers() const;

  GetPeers &operator=(GetPeers &&) = default;
  GetPeers &operator=(const GetPeers &) = default;

private:
  void check() const;

  static utils::Buffer make_internal(const utils::Buffer &transactionID,
                                     const utils::Buffer &token,
                                     const dht::NodeData &source,
                                     const utils::Buffer &nodeData);
};

template <class Container>
typename std::enable_if<
    std::is_same<typename std::remove_reference<Container>::type::value_type,
                 NodeSPtr>::value,
    utils::Buffer>::type
GetPeers::make(const utils::Buffer &transactionID, const utils::Buffer &token,
               const dht::NodeData &source, Container &&nodes_in) {
  utils::Buffer nodes;
  nodes.reserve(PACKED_NODE_SIZE * nodes_in.size());
  for (auto &node : nodes_in) {
    nodes += node->getPackedNode();
  };
  return make_internal(transactionID, token, source, nodes);
}

template <class Container>
typename std::enable_if<
    std::is_same<typename std::remove_reference<Container>::type::value_type,
                 udp::endpoint>::value,
    utils::Buffer>::type
GetPeers::make(const utils::Buffer &transactionID, const utils::Buffer &token,
               const dht::NodeData &source, Container &&endpoints) {
  BEncodeEncoder enc;
  enc.startDictionary();
  enc.addElement(Field::Reply);
  enc.startDictionary();
  enc.addDictionaryElement(Field::PeerID, source.write());

  enc.addDictionaryElement(torrentsync::dht::message::Field::Token, token);

  enc.addElement(torrentsync::dht::message::Field::Values);
  enc.startList();

  if (std::distance(std::begin(endpoints), std::end(endpoints)) == 0)
    throw dht::message::MessageException("GetPeers message has no peers",
                                         dht::message::ErrorType::serverError);

  for (const auto &endpoint : endpoints) {
    enc.addElement(dht::packEndpoint(endpoint));
  };

  enc.endList();

  enc.endDictionary();
  enc.addDictionaryElement(Field::TransactionID, transactionID);
  enc.addDictionaryElement(Field::Type, Type::Reply);
  enc.endDictionary();
  return enc.value();
}

bool isGetPeers(const BEncodeDecoder &);

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */
