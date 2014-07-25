
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

Distance Node::operator^( const Node& addr ) const noexcept
{
    Distance ret;
    ret.p1 = p1 ^ addr.p1;
    ret.p2 = p2 ^ addr.p2;
    ret.p3 = p3 ^ addr.p3;
    return ret;
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
            *(reinterpret_cast<int const*>(begin))));

    const uint16_t new_port =
        ntohs(
            *(reinterpret_cast<short const*>(begin+4)));

    _endpoint = udp::endpoint(new_address,new_port);
}

}; // dht
}; // torrentsync

