#pragma once

#include <torrentsync/dht/message/Message.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

/**
 * Due to how the protocol is made it's not possible to know just by the message
 * what type of message is being decoded if it's a reply.
 * Due to replies can be instantied also directly from a Message and should be 
 * done when they need to be used.
 */
//! Abstract class representing replies
class Reply : public Message
{
public:
    virtual ~Reply() = 0;

    Reply( const DataMap& );
    
    Reply( Message&& );
    
    Reply( const Message& );
    
    Reply( Reply&& ) = default;

    Reply( const Reply& ) = default;

    Reply& operator=( Reply&& ) = default;

    Reply& operator=( const Reply& ) = default;
};

} /* message */
} /* dht */
} /* torrentsync */

