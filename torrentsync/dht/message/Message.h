#pragma once

#include <string>

namespace torrentsync
{
namespace dht
{
namespace message
{

//! Abstract class representing every message
class Message
{
public:
    virtual const std::string& getMessage( 
            const std::string transactionID,
            const std::string source,
            const std::string destination,
            std::string& output) const =0;

    typedef enum
    {
        Query    = 'q',
        Response = 'r',
        Error    = 'e',
    } MessageType;

    typedef enum 
    {
        TransactionID = 't',
        Type          = 'y',
        QueryType     = 'q',
        ResponseType  = 'r',
        ErrorType     = 'e',
        Arguments     = 'a',
    } MessageFields;

    const MessageType type;

protected:

    Message( const MessageType type );
    virtual ~Message() {}
};

} /* message */
} /* dht */
} /* torrentsync */

