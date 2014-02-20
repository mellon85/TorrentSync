#include <torrentsync/dht/message/Message.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

namespace Type
{
    const char Query     = 'q';
    const char Response  = 'r';
    const char Error     = 'e';
};

namespace Field
{
    const char TransactionID = 't';
    const char Type          = 'y';
    const char QueryType     = 'q';
    const char ResponseType  = 'r';
    const char ErrorType     = 'e';
    const char Arguments     = 'a';

    const std::string NodeID = "id";
};


namespace Messages
{
    const std::string Ping   = "ping";
};

Message::Message()
{
}

const std::string& Message::getMessageType() const
{
    return Messages::Ping;
}

const char Message::getType() const
{
    return Type::Query;
}

} /* message */
} /* dht */
} /* torrentsync */
