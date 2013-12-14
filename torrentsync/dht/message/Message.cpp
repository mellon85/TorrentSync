#include <torrentsync/dht/message/Message.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

Message::Message(
    const MessageType type)
    : type(type)
{
}

const std::string&
Message::getMessage(
    const std::string transactionID,
    const std::string source,
    const std::string destination,
    std::string& output ) const
{
    // TODO add arguments
    
    return output;
}

} /* message */
} /* dht */
} /* torrentsync */
