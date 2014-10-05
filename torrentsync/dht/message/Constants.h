#pragma once

#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

using namespace torrentsync;

namespace Type
{
    extern const utils::Buffer Query;
    extern const utils::Buffer Reply;
    extern const utils::Buffer Error;
};

namespace Field
{
    // general
    extern const utils::Buffer PeerID;
    extern const utils::Buffer TransactionID;
    extern const utils::Buffer Type;
    extern const utils::Buffer Query;
    extern const utils::Buffer Reply;
    extern const utils::Buffer Error;
    extern const utils::Buffer Arguments;

    // find_node
    extern const utils::Buffer Target;
    extern const utils::Buffer Nodes;

    // get_peers
    extern const utils::Buffer InfoHash;
    extern const utils::Buffer Values;
    extern const utils::Buffer Token; // and announce
};

namespace Messages
{
    extern const utils::Buffer Ping;
    extern const utils::Buffer FindNode;
    extern const utils::Buffer GetPeers;
};

#define MAX_NODES_PER_GETPEERS 8
#define MAX_PEERS_PER_GETPEERS 32
#define PACKED_NODE_SIZE 26
#define PACKED_PEER_SIZE 6

namespace ErrorType
{
typedef enum
{
    genericError       = 201,
    serverError        = 202,
    protocolError      = 203,
    methodUnknownError = 204
} error_type;
};

} /* message */
} /* dht */
} /* torrentsync */

