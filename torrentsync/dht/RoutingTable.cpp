#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/dht/RoutingTable.h>

namespace torrentsync
{
namespace dht
{


RoutingTable::RoutingTable(
    const udp::endpoint& endpoint)
    : endpoint(endpoint)
    , table(AddressData::getRandom())
{
    LOG(INFO, "Node Address: " << table.getNodeAddress());
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

void RoutingTable::cleanupTable()
{
    assert(false);
}

}; // dht
}; // torrentsync

