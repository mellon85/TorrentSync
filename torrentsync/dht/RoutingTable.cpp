
#include <torrentsync/dht/RoutingTable.h>

namespace torrentsync
{
namespace dht
{


RoutingTable::RoutingTable(
    const udp::endpoint& endpoint)
    : endpoint(endpoint)
{
}

RoutingTable::~RoutingTable()
{
}

boost::asio::io_service& RoutingTable::getIO_service()
{
    return io_service;
}

const udp::endpoint& RoutingTable::getEndpoint() const
{
    return endpoint;
}

}; // dht
}; // torrentsync

