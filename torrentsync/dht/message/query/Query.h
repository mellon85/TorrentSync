#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/message/query/GetPeers.h>
#include <torrentsync/dht/message/query/Ping.h>

#include <boost/variant.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{
namespace Query
{

typedef boost::variant<
    torrentsync::dht::message::query::FindNode,
    torrentsync::dht::message::query::GetPeers,
    torrentsync::dht::message::query::Ping > Query;

};
};
};
};
