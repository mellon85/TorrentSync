#include <torrentsync/dht/Peer.h>
#include <torrentsync/utils/log/Logger.h>

namespace torrentsync
{
namespace dht
{

const size_t Peer::peerDataLength = 6;

Peer::Peer(
    const Node&       address,
    const udp::endpoint& endpoint )
        : Node(address)
        , _endpoint(endpoint)
{
    
}

void Peer::read(
    torrentsync::utils::Buffer::const_iterator begin,
    torrentsync::utils::Buffer::const_iterator end)
{
    NodeData::read(begin,end);
    begin += NodeData::addressDataLength;

    if ( end > begin && static_cast<size_t>(end-begin) < peerDataLength )
    {
        LOG(ERROR,"Peer - parsePeer: not enough data to parse. Expected " << peerDataLength << ", found: " << (end-begin) );
        throw std::invalid_argument("Not enough data to parse Peer contact information");
    }

    const boost::asio::ip::address_v4 new_address(
        ntohl(
            *(reinterpret_cast<int const*>(begin))));

    const uint16_t new_port =
        ntohs(
            *(reinterpret_cast<short const*>(begin+4)));

    _endpoint.address(new_address);
    _endpoint.port(new_port);

}

Peer::~Peer() {}

} /* dht */
} /* torrentsync */
