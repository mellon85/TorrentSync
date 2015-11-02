#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/message/query/GetPeers.h>
#include <torrentsync/dht/message/reply/Error.h>
#include <torrentsync/dht/message/reply/Ping.h>
#include <torrentsync/dht/message/reply/FindNode.h>

#include <map>

namespace torrentsync
{
namespace dht
{
namespace message
{

//! constructor
Message::Message(const DataMap& data) : _data(data)
{
}

const utils::Buffer Message::getType() const
{
    auto type = find( Field::Type );
    if (!type)
        throw MessageException("Couldn't find message type",
                ErrorType::protocolError);
    return *type;
}

const utils::Buffer Message::getTransactionID() const
{
    auto token = find( Field::TransactionID);
    if (!token)
        throw MessageException("Couldn't find token",
                ErrorType::protocolError);
    return *token;
}

const utils::Buffer Message::getID() const
{
    const utils::Buffer path =
        (getType() == Type::Reply ? Field::Reply : Field::Arguments)
            + "/" + Field::PeerID;
    auto id = find(path);
    if (!id)
        throw MessageException("Couldn't find peer id",
                ErrorType::protocolError);
    return *id;
}

const boost::optional<utils::Buffer> Message::find(
    const utils::Buffer& key) const
{
    boost::optional<utils::Buffer> ret;
    DataMap::const_iterator it = _data.find(key);
    if (it != _data.end())
        ret = it->second;
    return ret;
}

const std::string Message::string() const
{
    std::stringstream message;
    std::for_each( _data.begin(), _data.end(), [&](
        const DataMap::value_type& _i)
    {
        message << pretty_print(_i.first) << ":" << pretty_print(_i.second) << std::endl;
    });
    return message.str();
}

} /* message */
} /* dht */
} /* torrentsync */

std::ostream& operator<<(
    std::ostream& stream,
    const torrentsync::dht::message::Message& message)
{
    return stream << message.string();
}
