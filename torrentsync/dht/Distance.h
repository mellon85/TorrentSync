#pragma once

#include <torrentsync/dht/AddressData.h>

namespace torrentsync
{
namespace dht
{

class Address;

class Distance : public AddressData
{
public:
    Distance() {};
    ~Distance() {};
    
private:
    friend class torrentsync::dht::Address;
    
};

}; // dht
}; // torrentsync
