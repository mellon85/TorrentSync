#pragma once

#include <torrentsync/dht/message/Message.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

//! Abstract class representing every message
class Ping : public Message
{
public:
    Ping();

    static const std::string name;

    virtual const std::string& getMessage( 
            const std::string transactionID,
            const std::string source,
            const std::string destination,
            std::string& output) const =0;
private:


};

} /* message */
} /* dht */
} /* torrentsync */
