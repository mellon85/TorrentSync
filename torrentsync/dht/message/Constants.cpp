#include <torrentsync/dht/message/Constants.h>

namespace torrentsync {
namespace dht {
namespace message {

namespace Type {
const utils::Buffer Query = utils::makeBuffer("q");
const utils::Buffer Reply = utils::makeBuffer("r");
const utils::Buffer Error = utils::makeBuffer("e");
};

namespace Field {
const utils::Buffer PeerID = utils::makeBuffer("id");
const utils::Buffer TransactionID = utils::makeBuffer("t");
const utils::Buffer Type = utils::makeBuffer("y");
const utils::Buffer Reply = utils::makeBuffer("r");
const utils::Buffer Query = utils::makeBuffer("q");
const utils::Buffer Error = utils::makeBuffer("e");
const utils::Buffer Arguments = utils::makeBuffer("a");

const utils::Buffer Target = utils::makeBuffer("target");
const utils::Buffer Nodes = utils::makeBuffer("nodes");

const utils::Buffer InfoHash = utils::makeBuffer("info_hash");
const utils::Buffer Values = utils::makeBuffer("values");
const utils::Buffer Token = utils::makeBuffer("token");

const utils::Buffer ImpliedPort = utils::makeBuffer("implied_port");
const utils::Buffer Port = utils::makeBuffer("port");

const utils::Buffer Separator = utils::makeBuffer("/");
};

namespace Messages {
const utils::Buffer Ping = utils::makeBuffer("ping");
const utils::Buffer FindNode = utils::makeBuffer("find_node");
const utils::Buffer GetPeers = utils::makeBuffer("get_peers");
const utils::Buffer Announce = utils::makeBuffer("announce_peer");
};

} // message
} // dht
} // torrentsync
