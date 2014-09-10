#pragma once

#include <torrentsync/dht/message/Reply.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{

class NodeData;

namespace message
{
namespace reply
{

//! Abstract class representing every message
class Error : public dht::message::Reply
{
public:

    //! Ping constructor to initialize the class from a raw data map
    Error(const DataMap& dataMap);

    Error( Error&& ) = default;

    Error( Message&& );

    Error( const Message& );

    //! Destructor
    virtual ~Error() = default;

    //! creates a Error message
    //! @param transactionID the ID
    //! @param source source address (should be our own address)
    static const utils::Buffer make( 
        const utils::Buffer& transactionID,
        const ErrorType::error_type error);

    Error& operator=( Error&& ) = default;

private:

    void check() const;
};
 
} // query
} // message
} // dht
} // torrentsync

