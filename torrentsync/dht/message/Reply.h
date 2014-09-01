#pragma once

#include <torrentsync/dht/message/Message.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

//! Abstract class representing replies
class Reply : public Message
{
public:
    virtual ~Reply() = 0;

    Reply( const DataMap& data );
    
    Reply( Reply&& ) = default;

    Reply( const Reply& ) = default;

    Reply& operator=( Reply&& ) = default;

    Reply& operator=( const Reply& ) = default;
};

} /* message */
} /* dht */
} /* torrentsync */

