#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/Ping.h>

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

    const std::string PeerID = "id";
};

namespace Messages
{
    const std::string Ping     = "ping";
    const std::string FindNode = "find_node";
};

boost::shared_ptr<Message> Message::parseMessage( std::istream& istream )
{
    BEncodeDecoder decoder;
    decoder.parseMessage(istream);
    
    auto type = find( Field::Type, decoder.getData() );
    if (!type)
        throw MalformedMessageException("Couldn't find message type");

    auto msgType = find( *type == Type::Query ? Field::QueryType : Field::ResponseType, decoder.getData() );
    if (!msgType)
        throw MalformedMessageException("Couldn't find message name");

    boost::shared_ptr<Message> message;
    if (*msgType == Messages::Ping)
    {
        message.reset(new Ping(decoder.getData()));
        return message;
    }
    else
    {
        throw MalformedMessageException("Unknown message name");
    }

    return message;
}

const std::string Message::getMessageType() const
{
    std::string type = getType();
    auto msgType = find( type == Type::Query ? Field::QueryType : Field::ResponseType, data );
    if (!msgType)
        throw MalformedMessageException("Couldn't find message name");
    return msgType->get();
}

const std::string Message::getType() const
{
    boost::optional<torrentsync::utils::Buffer> type;
    type = find( Field::Type, data );
    if (!type)
        throw MalformedMessageException("Couldn't find message type");
    return type->get();
}

boost::optional<torrentsync::utils::Buffer> Message::find(
    const std::string& key,
    const DataMap& data)
{
    boost::optional<torrentsync::utils::Buffer> ret;

    DataMap::const_iterator it = data.find(key); 
    if ( it != data.end() )
    {
        ret = it->second;
    }

    return ret;
}

} /* message */
} /* dht */
} /* torrentsync */
