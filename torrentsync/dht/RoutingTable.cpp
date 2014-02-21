#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/dht/RoutingTable.h>

namespace torrentsync
{
namespace dht
{

RoutingTable::RoutingTable(
    const udp::endpoint& endpoint)
    : _table(AddressData::getRandom())
    , _socket(io_service,endpoint)
{
    LOG(INFO, "RoutingTable * Node Address: " << _table.getNodeAddress());
    LOG(INFO, "RoutingTable * Bind Address: " << endpoint);
}

RoutingTable::~RoutingTable()
{
}

boost::asio::io_service& RoutingTable::getIO_service()
{
    return io_service;
}

const udp::endpoint RoutingTable::getEndpoint() const
{
    return _socket.local_endpoint();
}

void initializeTable()
{
    // TODO register a timer at the IO_service to call a function to consume
    // the initial_address data
    assert(false);
}

void RoutingTable::cleanupTable()
{
    assert(false);
}

}; // dht
}; // torrentsync

