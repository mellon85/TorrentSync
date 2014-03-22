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
    Ping(const DataMap& dataMap) : Message(dataMap) {}

    virtual ~Ping() {}

    static const torrentsync::utils::Buffer getMessage( 
        const torrentsync::utils::Buffer& transactionID,
        const torrentsync::dht::NodeData& source);

    torrentsync::utils::Buffer getID();

    torrentsync::utils::Buffer getToken();

};

} /* message */
} /* dht */
} /* torrentsync */
