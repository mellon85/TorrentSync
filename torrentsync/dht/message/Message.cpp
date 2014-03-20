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

    const std::string PeerID = "id";
};

namespace Messages
{
    const std::string Ping   = "ping";
};

boost::shared_ptr<Message> Message::parseMessage( std::istream& istream )
{
    boost::shared_ptr<Message> message( new Message() );
    (static_cast<BEncodeDecoder*>(message.get()))->parseMessage(istream);
    return message;
}

const std::string Message::getMessageType() const
{
    std::string type = getType();
    auto msgType = find( type == Type::Query ? Field::QueryType : Field::ResponseType );
    if (!msgType)
        throw MalformedMessageException("Couldn't find message name");
    return msgType->get();
}

const std::string Message::getType() const
{
    boost::optional<torrentsync::utils::Buffer> type;
    type = find( Field::Type );
    if (!type)
        throw MalformedMessageException("Couldn't find message type");
    return type->get();
}

} /* message */
} /* dht */
} /* torrentsync */
