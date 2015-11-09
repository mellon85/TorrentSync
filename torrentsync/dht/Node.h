#pragma once

#include <atomic>

#include <boost/asio.hpp>

#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Distance.h>
#include <torrentsync/dht/message/Constants.h>

namespace torrentsync {
namespace dht {

using boost::asio::ip::udp;

utils::Buffer packEndpoint(const udp::endpoint &endpoint);
udp::endpoint unpackEndpoint(torrentsync::utils::Buffer::const_iterator begin);

//! A DHT address with the associated statistics and informations
class Node : public NodeData {
public:
  //! Copy constructor
  Node(const Node &addr) = default;
  Node(Node &&) = default;

  Node(const torrentsync::utils::Buffer &,
       const boost::optional<udp::endpoint> & =
           boost::optional<udp::endpoint>());

  template <class Iterator> Node(Iterator begin, Iterator end);

  Node &operator=(Node &&) = default;
  Node &operator=(const Node &) = default;

  ~Node() = default;

  //! marks the address as good/fresh
  void setGood() noexcept;

  //! Is the address good/fresh?
  //! @return true if good/fresh
  bool isGood() const noexcept;

  bool isQuestionable() const noexcept;
  bool isBad() const noexcept;
  time_t getLastTimeGood() const noexcept;

  static constexpr time_t good_interval = 15*60; // 15 minutes

  //! Maximum number of unanswered queries
  static constexpr size_t allowed_unanswered_queries = 10;

  //! returns the Peer's endpoint
  const boost::optional<udp::endpoint> &getEndpoint() const noexcept;

  void setEndpoint(const udp::endpoint &);

  //! Parses a node information from the Buffer.
  //! In this class it implements the parsing of the actual ip address.
  //! @param begin the beginning of the data in the iterator
  //! @param end the beginning of the data in the iterator
  //! @throws std::invalid_argument in case the data is not correct
  template <class Iterator> void read(Iterator begin, Iterator end);

  /** Returns a packed representation of the node
   * 20-byte id followed by network order bytes representing the ipv4 address
   * and the port number.
   * @return the packed representation.
   */
  utils::Buffer getPackedNode() const;

  /** Returns a packed representation of the node
   * 4-byte ipv4 address followed by the port, all in network order bytes
   * @return the packed representation.
   */
  utils::Buffer getPackedPeer() const;

protected:
  Node();

  //! the last time the node was set as good
  time_t _last_time_good;

  //! Number of the last unanswered queries
  uint8_t _last_unanswered_queries;

  //! the endpoint of the node
  boost::optional<udp::endpoint> _endpoint;
};

typedef std::shared_ptr<Node> NodeSPtr;

template <class Iterator> Node::Node(Iterator begin, Iterator end) : Node() {
  read(begin, end);
}

template <class Iterator> void Node::read(Iterator begin, Iterator end) {
  NodeData::read(begin, end);
  begin += NodeData::addressDataLength;

  if (end > begin && static_cast<size_t>(end - begin) < PACKED_PEER_SIZE) {
    throw std::invalid_argument(
        "Not enough data to parse Peer contact information");
  }

  _endpoint = unpackEndpoint(begin);
}

}; // dht
}; // torrentsync

namespace std {
template <>
struct less<torrentsync::dht::NodeSPtr>
    : public std::binary_function<torrentsync::dht::NodeSPtr,
                                  torrentsync::dht::NodeSPtr, bool> {
  bool operator()(const torrentsync::dht::NodeSPtr &x,
                  const torrentsync::dht::NodeSPtr &y) const {
    return *x < *y;
  }
};
}; // std
