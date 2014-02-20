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
    extern const std::string Query;
    extern const std::string Response;
    extern const std::string Error;
};

namespace Field
{
    extern const std::string TransactionID;
    extern const std::string Type;
    extern const std::string QueryType;
    extern const std::string ResponseType;
    extern const std::string ErrorType;
    extern const std::string Arguments;

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
    const std::string getType() const;

protected:

    Message();
    virtual ~Message() {}
};

} /* message */
} /* dht */
} /* torrentsync */

