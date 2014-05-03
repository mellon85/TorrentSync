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

//! Abstract class representing every message
class Ping : public Message
{
public:
    //! Ping constructor to initialize the class from a raw data map
    Ping(const DataMap& dataMap) : Message(dataMap) {}

    //! Destructor
    virtual ~Ping() {}

    //! creates a Ping message
    //! @param transactionID the ID
    //! @param source source address (should be our own address)
    static const torrentsync::utils::Buffer getMessage( 
        const torrentsync::utils::Buffer& transactionID,
        const torrentsync::dht::NodeData& source);

    //! returns the message's ID
    //! @return ID
    torrentsync::utils::Buffer getID();

    //! returns message's Token
    //! @return Token
    torrentsync::utils::Buffer getToken();

};

} /* message */
} /* dht */
} /* torrentsync */
