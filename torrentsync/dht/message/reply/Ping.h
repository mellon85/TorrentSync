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
class Ping : public dht::message::Reply
{
public:
    //! Ping constructor to initialize the class from a raw data map
    Ping(const DataMap& dataMap);
    
    Ping( Ping&& ) = default;
    
    //! Destructor
    virtual ~Ping() {}

    //! creates a Ping message
    //! @param transactionID the ID
    //! @param source source address (should be our own address)
    static const utils::Buffer make( 
        const utils::Buffer& transactionID,
        const dht::NodeData& address);

    Ping& operator=( Ping&& ) = default;
};
 
} /* query */
} /* message */
} /* dht */
} /* torrentsync */
