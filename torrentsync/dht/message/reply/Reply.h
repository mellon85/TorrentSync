#include <torrentsync/dht/message/reply/Error.h>
#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/dht/message/reply/GetPeers.h>
#include <torrentsync/dht/message/reply/Ping.h>

#include <boost/variant.hpp>

namespace torrentsync {
namespace dht {
namespace message {
namespace reply {

typedef boost::variant<torrentsync::dht::message::reply::Error,
                       torrentsync::dht::message::reply::FindNode,
                       torrentsync::dht::message::reply::GetPeers,
                       torrentsync::dht::message::reply::Ping> Reply;
};
};
};
};
