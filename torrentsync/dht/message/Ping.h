#pragma once

#include <torrentsync/dht/message/Message.h>


namespace torrentsync
{
namespace utils
{
class Buffer;
}; // utils

namespace dht
{

class NodeData;

namespace message
{

//! Abstract class representing every message
class Ping : public Message
{
public:
    Ping();

    static const std::string& getMessage( 
            const std::string& transactionID,
            const torrentsync::dht::NodeData& source,
            std::string& output);

    torrentsync::utils::Buffer getID();

    torrentsync::utils::Buffer getToken();

};

} /* message */
} /* dht */
} /* torrentsync */
