#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/Constants.h>
#include <torrentsync/utils/log/Logger.h>

#include <boost/integer_traits.hpp>

namespace torrentsync {
namespace dht {

const time_t Node::good_interval = 15 * 60; // 15 minutes

utils::Buffer packEndpoint(const udp::endpoint &endpoint) {
  utils::Buffer buff;
  buff.reserve(PACKED_PEER_SIZE);

  auto networkOrderAddress = htonl(endpoint.address().to_v4().to_ulong());
  const uint16_t portNetworkOrder = htons(endpoint.port());

  buff.push_back(networkOrderAddress);
  buff.push_back(networkOrderAddress >> 8);
  buff.push_back(networkOrderAddress >> 16);
  buff.push_back(networkOrderAddress >> 24);

  buff.push_back(portNetworkOrder);
  buff.push_back(portNetworkOrder >> 8);

  return buff;
}

udp::endpoint unpackEndpoint(torrentsync::utils::Buffer::const_iterator begin) {
  const uint32_t address = *((uint32_t *)&*begin);
  begin += sizeof(address);

  const boost::asio::ip::address_v4 new_address(ntohl(address));

  const uint16_t port = *((uint16_t *)&*begin);

  udp::endpoint ep(new_address, ntohs(port));
  return ep;
}

Node::Node() { setGood(); }

Node::Node(const torrentsync::utils::Buffer &data,
           const boost::optional<udp::endpoint> &endpoint)
    : NodeData(data), _endpoint(endpoint) {
  setGood();
}

void Node::setGood() noexcept {
  _last_time_good = time(nullptr);
  _last_unanswered_queries = 0;
}

bool Node::isGood() const noexcept {
  return _last_time_good > time(nullptr) - good_interval;
}

bool Node::isQuestionable() const noexcept {
  return !isGood() && _last_unanswered_queries <= allowed_unanswered_queries;
}

bool Node::isBad() const noexcept {
  return !isGood() && _last_unanswered_queries > allowed_unanswered_queries;
}

const time_t &Node::getLastTimeGood() const noexcept { return _last_time_good; }

const boost::optional<udp::endpoint> &Node::getEndpoint() const noexcept {
  return _endpoint;
}

void Node::setEndpoint(const udp::endpoint &endpoint) { _endpoint = endpoint; }

utils::Buffer Node::getPackedNode() const {
  utils::Buffer buff = NodeData::write();
  buff.reserve(PACKED_NODE_SIZE);

  auto peer = getPackedPeer();

  buff.insert(buff.end(), peer.begin(), peer.end());
  return buff;
}

utils::Buffer Node::getPackedPeer() const {
  assert(!!_endpoint);
  return packEndpoint(*_endpoint);
}

}; // dht
}; // torrentsync
