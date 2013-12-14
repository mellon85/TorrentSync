#include <torrentsync/dht/message/Ping.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

const std::string Ping::name = "ping";

Ping::Ping()
    : Message( MessageType::Query )
{
}

const std::string&
Ping::getMessage( 
        const std::string transactionID,
        const std::string source,
        const std::string destination,
        std::string& output) const
{
    // start part of the message
    Message::getMessage(transactionID,source,destination,output);

    // TODO add arguments
    
    return output;
}

} /* message */
} /* dht */
} /* torrentsync */

