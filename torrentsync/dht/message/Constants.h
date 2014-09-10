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
};

namespace Messages
{
    extern const utils::Buffer Ping;
    extern const utils::Buffer FindNode;
};

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

