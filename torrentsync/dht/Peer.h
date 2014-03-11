#pragma once

#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <torrentsync/dht/Node.h>
#include <torrentsync/utils/Buffer.h>

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
class Peer : public Node {
public:

    Peer(
        const Node&       address,
        const udp::endpoint& endpoint);

    //! default empty constructor
    Peer() {};

    //! destructor
    ~Peer();

    //! returns the Peer's endpoint
    const udp::endpoint& getEndpoint() const { return _endpoint; }

    static const size_t peerDataLength;

    //! Parses a node information from the Buffer.
    //! In this class it implements the parsing of the actual ip address.
    //! @param begin the beginning of the data in the iterator
    //! @param end the beginning of the data in the iterator
    //! @throws std::invalid_argument in case the data is not correct
    void read(
        torrentsync::utils::Buffer::const_iterator begin,
        torrentsync::utils::Buffer::const_iterator end);
    
private:

    //! the endpoint of the node
    udp::endpoint _endpoint;

};

typedef boost::shared_ptr<Peer> PeerSPtr;

} /* dht */    
} /* torrentsync */
