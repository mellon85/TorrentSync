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

    ~Node () {};

    inline       udp::endpoint& getEndpoint();
    inline const udp::endpoint& getEndpoint() const;
    
private:

    //! the endpoint of the node
    udp::endpoint endpoint;
};

typedef boost::shared_ptr<Node> NodeSPtr;

udp::endpoint& Node::getEndpoint()
{
    return endpoint;
}

const udp::endpoint& Node::getEndpoint() const
{
    return endpoint;
}

    
} /* dht */    
} /* torrentsync */
