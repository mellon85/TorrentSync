#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/Ping.h>
#include <torrentsync/dht/message/FindNode.h>

#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>

#include <map>

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

//! constructor
Message::Message(const DataMap& data) : _data(data)
{
}

/*
Message::Message( Message&& message )
{
    _data = std::move(message._data);
}
*/

std::shared_ptr<Message> Message::parseMessage( const utils::Buffer& buffer )
{
    return parseMessage(buffer,buffer.size());
}

std::shared_ptr<Message> Message::parseMessage( const utils::Buffer& buffer, const size_t length )
{
    bio::array_source source(reinterpret_cast<const char*>(buffer.data()),length);
    bio::stream<bio::array_source> in(source);
    return parseMessage(in);
}

std::shared_ptr<Message> Message::parseMessage( std::istream& istream )
{
    BEncodeDecoder decoder;
    try
    {
        decoder.parseMessage(istream);
    }
    catch( const BEncodeException& e )
    {
        std::stringstream ss;
        ss << "Couldn't parse message: " << e.what();
        throw MalformedMessageException(ss.str());
    }
    
    auto type = find( Field::Type, decoder.getData() );
    if (!type)
        throw MalformedMessageException("Couldn't find message type");

    std::shared_ptr<Message> message;
    if (*type == Type::Query)
    {
        auto msgType = find(Field::Query, decoder.getData() );
        if (!msgType)
            throw MalformedMessageException("Couldn't find message name");
        
        if( *msgType == Messages::Ping)
        {
            message.reset(new Ping(decoder.getData()));
        }
        else if ( *msgType == Messages::FindNode )
        {
            message.reset(new FindNode(decoder.getData()));
        }
        else
        {
            throw MalformedMessageException("Unknown message name");
        }
    }
    else
    {
        //@TODO use validators to verify the message or 
        // create classes for the replies.
        message.reset(new Message(decoder.getData()));
    }
    return message;
}

const boost::optional<utils::Buffer> Message::getMessageType() const
{
    return find(Field::Query);
}

const utils::Buffer Message::getType() const
{
    auto type = find( Field::Type );
    if (!type)
        throw MalformedMessageException("Couldn't find message type");
    return *type;
}

const utils::Buffer Message::getTransactionID() const
{
    auto token = find( Field::TransactionID);
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

const utils::Buffer Message::getID() const
{
    const std::string path =
        (getType() == Type::Reply ? Field::Reply : Field::Arguments) 
            + "/" + Field::PeerID;
    auto id = find(path);
    if (!id)
        throw MalformedMessageException("Couldn't find peer id");
    return *id;
}

const boost::optional<utils::Buffer> Message::find(
    const std::string& key) const
{
    return find(key,_data);
}

const std::string Message::string() const
{
    std::stringstream message;
    std::for_each( _data.begin(), _data.end(), [&](
        const DataMap::value_type& t)
    {
        message << t.first << ":" << pretty_print(t.second) << std::endl;
    });
    return message.str();
}

const boost::optional<utils::Buffer> Message::find(
    const std::string& key,
    const DataMap& data)
{
    boost::optional<utils::Buffer> ret;
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

std::ostream& operator<<(
    std::ostream& stream,
    const torrentsync::dht::message::Message& message)
{
    return stream << message.string();
}
