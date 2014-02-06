#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <torrentsync/dht/Address.h>

namespace torrentsync
{
namespace dht
{

namespace message
{
class Message;
} /* message */

using boost::asio::ip::udp;

//! Class representing a DHT node
class Node : public Address {
public:
    Node(
        const Address&       address,
        const udp::endpoint& endpoint);

    ~Node();

    //udp::endpoint& getEndpoint();

    const udp::endpoint& getEndpoint() const { return endpoint; }
    
private:

    //! the endpoint of the node
    udp::endpoint endpoint;
};

typedef boost::shared_ptr<Node> NodeSPtr;

} /* dht */    
} /* torrentsync */
