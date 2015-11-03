#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/utils/Buffer.h>


namespace torrentsync
{
namespace dht
{

class NodeData;

namespace message
{
namespace query
{

//! Abstract class representing every message
class Ping : public dht::message::Message
{
public:
    //! Ping constructor to initialize the class from a raw data map
    Ping(const DataMap& dataMap);

    Ping( Ping&& ) = default;
    Ping( const Ping& ) = default;

    //! Destructor
    ~Ping() = default;

    //! creates a Ping message
    //! @param transactionID the ID
    //! @param source source address (should be our own address)
    static const utils::Buffer make(
        const utils::Buffer& transactionID,
        const dht::NodeData& address);

    Ping& operator=( Ping&& ) = default;
    Ping& operator=( const Ping& ) = default;
};

} /* query */
} /* message */
} /* dht */
} /* torrentsync */
