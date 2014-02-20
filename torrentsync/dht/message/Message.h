#pragma once

#include <string>

#include <boost/shared_ptr.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{

namespace Type
{
    extern const char Query;
    extern const char Response;
    extern const char Error;
};

namespace Field
{
    extern const char TransactionID;
    extern const char Type;
    extern const char QueryType;
    extern const char ResponseType;
    extern const char ErrorType;
    extern const char Arguments;

    extern const std::string NodeID;
};

namespace Messages
{
    extern const std::string Ping;
};

typedef enum
{
    Ping,
    FindNode,
    GetPeers,
    Announce,
} Name;

//! Abstract class representing every message
class Message
{
public:

    //! parse a generic message and returns an instance of it
    boost::shared_ptr<Message> parseMessage( std::istream& );

    //! Returns the message type. In this way you can cast to the correct
    //! object.
    //! @return a member of the Messages namespace
    const std::string& getMessageType() const;

    //! returns the type of the message
    //! @return a member of Type namespace
    const char getType() const;

protected:

    Message();
    virtual ~Message() {}
};

} /* message */
} /* dht */
} /* torrentsync */

