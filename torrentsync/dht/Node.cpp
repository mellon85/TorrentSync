
#include <torrentsync/dht/Node.h>

#include <boost/integer_traits.hpp>

namespace torrentsync
{
namespace dht
{

const time_t Node::good_interval              = 15 * 60;  // 15 minutes
const size_t Node::allowed_unanswered_queries = 10;

Node::Node(const std::string& str)
{
    parseString(str);
    setGood();
}

Node::Node( const Node& addr )
{
    *this = addr;
    setGood();
}

const torrentsync::utils::Buffer Node::getTransactionID() const
{
    return _transacton_id;
}

torrentsync::utils::Buffer& Node::accessTransactionID()
{
    return _transacton_id;
}
}; // dht
}; // torrentsync

