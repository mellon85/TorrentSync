#include <torrentsync/dht/message/Query.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

//! constructor
Query::Query(const DataMap& data) : Message(data)
{
}

const utils::Buffer Query::getMessageType() const
{
    const std::string path = Type::Query;
    auto id = find(path);
    if (!id)
        throw MalformedMessageException("Couldn't find message type",
                ErrorType::protocolError);
    return *id;
}

} /* message */
} /* dht */
} /* torrentsync */
