#include <torrentsync/dht/Node.h>

namespace torrentsync
{
namespace dht
{

Node::Node(
    const Address&       address,
    const udp::endpoint& endpoint )
        : Address(address)
        , endpoint(endpoint)
{
    
}

} /* dht */
} /* torrentsync */
