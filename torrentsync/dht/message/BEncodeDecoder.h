#pragma once

#include <list>
#include <string>
#include <iostream>
#include <sstream>

namespace torrentsync
{
namespace dht
{
namespace message
{

//! root is always a dictionary
class BEncodeDecoder
{
    public:

    BEncodeDecoder() {}

    

private:
    std::string lastKey;
    std::stringstream result;

};

} // torrentsync
} // dht
} // message

