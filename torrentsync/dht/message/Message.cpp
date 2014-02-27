#include <torrentsync/dht/message/Message.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

namespace Type
{
    const std::string Query     = "q";
    const std::string Response  = "r";
    const std::string Error     = "e";
};

namespace Field
{
    const std::string TransactionID = "t";
    const std::string Type          = "y";
    const std::string QueryType     = "q";
    const std::string ResponseType  = "r";
    const std::string ErrorType     = "e";
    const std::string Arguments     = "a";

    const std::string NodeID = "id";
};

namespace Messages
{
    const std::string Ping   = "ping";
};

const std::string& Message::getMessageType() const
{
    assert(false);
    return Messages::Ping;
}

const std::string Message::getType() const
{
    assert(false);
    return Type::Query;
}

} /* message */
} /* dht */
} /* torrentsync */
