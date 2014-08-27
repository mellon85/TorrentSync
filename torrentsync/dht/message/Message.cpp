#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/Ping.h>
#include <torrentsync/dht/message/FindNode.h>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{

namespace Type
{
    const std::string Query     = "q";
    const std::string Reply     = "r";
    const std::string Error     = "e";
};

namespace Field
{
    const std::string PeerID        = "id";
    const std::string TransactionID = "t";
    const std::string Type          = "y";
    const std::string Reply         = "r";
    const std::string Query         = "q";
    const std::string ErrorType     = "e";
    const std::string Arguments     = "a";

    const std::string Target        = "target";
    const std::string Nodes         = "nodes";
};

namespace Messages
{
    const std::string Ping     = "ping";
    const std::string FindNode = "find_node";
};

namespace bio = boost::iostreams;

typedef bio::stream<bio::array_source> array_stream;

std::shared_ptr<Message> Message::parseMessage( const torrentsync::utils::Buffer& buffer )
{
    bio::array_source source(buffer.cbegin(),buffer.cend());
    bio::stream<bio::array_source> in(source);
    return parseMessage(in);
}

std::shared_ptr<Message> Message::parseMessage( const torrentsync::utils::Buffer& buffer, const size_t length )
{
    bio::array_source source(buffer.cbegin(),length);
    bio::stream<bio::array_source> in(source);
    return parseMessage(in);
}

std::shared_ptr<Message> Message::parseMessage( std::istream& istream )
{
    BEncodeDecoder decoder;
    decoder.parseMessage(istream);
    
    auto type = find( Field::Type, decoder.getData() );
    if (!type)
        throw MalformedMessageException("Couldn't find message type");

    auto msgType = find( *type == Type::Query ? Field::Query : Field::Reply, decoder.getData() );
    if (!msgType)
        throw MalformedMessageException("Couldn't find message name");

    std::shared_ptr<Message> message;
    if( *msgType == Messages::Ping)
    {
        message.reset(new Ping(decoder.getData()));
        return message;
    }
    else if ( *msgType == Messages::FindNode )
    {
        message.reset(new FindNode(decoder.getData()));
        return message;
    } else
    {
        throw MalformedMessageException("Unknown message name");
    }
}

const std::string Message::getMessageType() const
{
    std::string type = getType();
    auto msgType = find( type == Type::Query ? Field::Query : Field::Reply, data );
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

torrentsync::utils::Buffer Message::getTransactionID() const
{
    boost::optional<torrentsync::utils::Buffer> token;
    token = find( Field::TransactionID, data );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

torrentsync::utils::Buffer Message::getID() const
{
    const std::string path =
        (getType() == Type::Reply ? Field::Reply : Field::Arguments) 
            + "/" + Field::PeerID;
    boost::optional<torrentsync::utils::Buffer> id;
    id = find(path,data);
    if (!id)
        throw MalformedMessageException("Couldn't find peer id");
    return *id;
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
