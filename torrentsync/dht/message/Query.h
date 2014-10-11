#pragma once

#include <string>

#include <memory>
#include <torrentsync/dht/message/BEncodeDecoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/Message.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

using namespace torrentsync;

//! Abstract class representing every query
class Query : public Message
{
public:
    virtual ~Query() = default;

    Query( Query&& ) = default;

    Query( const Query& ) = default;

    //! Returns the message type. In this way you can cast to the correct
    //! object.
    //! @return a member of the Querys namespace if it's a query. If it's a reply it will be empty
    //! @throw MessageException in case the field is not available.
    const utils::Buffer getMessageType() const;

    Query& operator=( Query&& ) = default;

    Query& operator=( Query& ) = default;

protected:
    //! Default constructor
    Query() = default;

    //! Constructor 
    Query(const DataMap& data);
};

} /* message */
} /* dht */
} /* torrentsync */
