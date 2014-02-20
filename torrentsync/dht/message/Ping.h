#pragma once

#include <torrentsync/dht/message/Message.h>


namespace torrentsync
{
namespace dht
{

class AddressData;

namespace message
{

//! Abstract class representing every message
class Ping : public Message
{
public:
    Ping();

    static const std::string& getMessage( 
            const std::string& transactionID,
            const torrentsync::dht::AddressData& source,
            std::string& output);

};

} /* message */
} /* dht */
} /* torrentsync */
