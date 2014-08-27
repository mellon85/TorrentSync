#include <torrentsync/dht/Node.h>
#include <torrentsync/utils/log/Logger.h>

#include <boost/integer_traits.hpp>

namespace torrentsync
{
namespace dht
{

const time_t Node::good_interval              = 15 * 60;  // 15 minutes
const size_t Node::allowed_unanswered_queries = 10;
#define PEERDATALENGTH 6

Node::Node( const Node& addr )
{
    *this = addr;
    setGood();
}

Node::Node(
    const torrentsync::utils::Buffer& data,
    const boost::optional<udp::endpoint>& endpoint ) :
     NodeData(data), _endpoint(endpoint)
{
    setGood();
}

Node Node::parse( const std::string& str )
{
    Node node;
    node.parseString(str);
    node.setGood();
    return node;
}

void Node::setGood() noexcept
{
    _last_time_good = time(0);
    _last_unanswered_queries = 0;
}

bool Node::isGood() const noexcept
{
    return _last_time_good > time(0)-good_interval;
}

bool Node::isQuestionable() const noexcept
{
    return !isGood() && _last_unanswered_queries <= allowed_unanswered_queries;
}

bool Node::isBad() const noexcept
{
    return !isGood() && _last_unanswered_queries >  allowed_unanswered_queries;
}

const time_t& Node::getLastTimeGood() const noexcept
{
    return _last_time_good;
}

const boost::optional<udp::endpoint>& Node::getEndpoint() const noexcept
{
    return _endpoint;
}

void Node::setEndpoint( udp::endpoint& endpoint )
{
    _endpoint = endpoint;
}

void Node::read(
    torrentsync::utils::Buffer::const_iterator begin,
    torrentsync::utils::Buffer::const_iterator end)
{
    NodeData::read(begin,end);
    begin += NodeData::addressDataLength;

    if ( end > begin && static_cast<size_t>(end-begin) < PEERDATALENGTH)
    {
        LOG(ERROR,"Peer - parsePeer: not enough data to parse. Expected " << PEERDATALENGTH << ", found: " << (end-begin) );
        throw std::invalid_argument("Not enough data to parse Peer contact information");
    }

    const boost::asio::ip::address_v4 new_address(
        ntohl(
            *(reinterpret_cast<uint32_t const*>(begin))));

    const uint16_t new_port =
        ntohs(
            *(reinterpret_cast<uint16_t const*>(begin+4)));

    _endpoint = udp::endpoint(new_address,new_port);
}

utils::Buffer Node::getPackedNode() const
{
    assert(!!_endpoint);
    
    utils::Buffer buff = NodeData::write();
    
    buff.resize(PACKED_NODE_SIZE);
    
    const std::array<uint8_t, 4> networkOrderAddress = _endpoint->address().to_v4().to_bytes();
    const uint16_t portNetworkOrder    = htons(_endpoint->port());

    for( int i = 0; i < 4; ++i)
        buff[20+i] = networkOrderAddress[i];
    buff[24] = static_cast<uint8_t>(portNetworkOrder & 0xFF);
    buff[25] = static_cast<uint8_t>(portNetworkOrder >> 8);
    
    return buff;
}

}; // dht
}; // torrentsync

