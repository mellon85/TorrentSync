#include <torrentsync/utils/Buffer.h>
#include <iostream>
#include <sstream>
#include <cassert>
#include <boost/lexical_cast.hpp>

namespace torrentsync
{
namespace utils
{

Buffer makeBuffer( const std::string& str)
{
    return Buffer(str.cbegin(),str.cend());
}

Buffer makeBuffer( const uint64_t& v )
{
    const std::string str = boost::lexical_cast<std::string>(v);
    return Buffer(str.cbegin(),str.cend());
}

std::string toString( const Buffer& buff )
{
    return std::string(buff.cbegin(),buff.cend());
}

const torrentsync::utils::Buffer parseIDFromHex( const std::string& str )
{
    torrentsync::utils::Buffer buff;
    assert(str.size() % 2 == 0);
    for( size_t i = 0;  i < str.size(); i += 2 )
    {
        std::stringstream ss;
        ss << std::hex << str.substr(i,2);
        int v;
        ss >> v;
        buff.push_back(v);
    }
    return buff;
}

}; // utils
}; // torrentsync

bool operator==(
    const torrentsync::utils::Buffer& buff,
    const std::string& str)
{
    auto itB = buff.cbegin();
    auto itS = str.cbegin();
    if (buff.size() != str.size())
    {
        return false;
    }
    else
    {
        while (itB != buff.cend())
        {
            if ( *itB++ != *itS++ ) 
                return false;
        }
        return true;
    }
}

bool operator!=(
    const torrentsync::utils::Buffer& buff,
    const std::string& str)
{
    return ! (buff == str);
}

std::ostream& operator<<(
    std::ostream& stream,
    const torrentsync::utils::Buffer& buff )
{
    for( auto it = buff.cbegin(); it != buff.cend(); ++it )
        stream << *it;
    return stream;
};

std::ostream& operator<<(
    std::ostream& stream,
    const pretty_print& buff )
{
    const auto flags = stream.flags();
    stream << std::hex;
    for( auto it = buff._buff.cbegin(); it != buff._buff.cend(); ++it )
    {
        if ( std::isprint(*it))
        {
            stream << *it;
        }
        else
        {
            stream << "0x" << (static_cast<uint8_t>(*it) & 0xFF);
        }
    }
    stream.setf(flags);
    return stream;
}

