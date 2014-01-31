
#include <torrentsync/dht/RoutingTable.h>

namespace torrentsync
{
namespace dht
{


RoutingTable::RoutingTable(
    const udp::endpoint& endpoint )
    : endpoint(endpoint)
{
}

RoutingTable::~RoutingTable()
{
}

}; // dht
}; // torrentsync

