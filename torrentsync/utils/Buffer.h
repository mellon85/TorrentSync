#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <algorithm>

namespace torrentsync
{
namespace utils
{

typedef std::vector<uint8_t> Buffer;

Buffer makeBuffer( const std::string& );
Buffer makeBuffer( const uint64_t& );
Buffer makeBuffer( const uint8_t*, const size_t );

std::string toString( const Buffer& );

const Buffer parseIDFromHex( const std::string& str );

}; // utils
}; // torrentsync

bool operator==( const torrentsync::utils::Buffer&, const std::string& );
//bool operator!=( const torrentsync::utils::Buffer&, const std::string& );

//std::ostream& operator<<( std::ostream& stream, const torrentsync::utils::Buffer& buff );

//! Pretty printing wrapper for a buffer containing string data.
//! Will print all the content of the buffer escaping the binary data
//! until a c-string terminator ('\0') is found.
struct pretty_print
{
    const torrentsync::utils::Buffer& _buff;
    pretty_print(const torrentsync::utils::Buffer& buff);
};

std::ostream& operator<<( std::ostream& stream, const pretty_print& buff );

torrentsync::utils::Buffer operator+(
    const torrentsync::utils::Buffer&,
    const torrentsync::utils::Buffer&);

torrentsync::utils::Buffer operator+(
    const torrentsync::utils::Buffer&,
    const std::string&);

torrentsync::utils::Buffer& operator+=(
    torrentsync::utils::Buffer&,
    const torrentsync::utils::Buffer&);

torrentsync::utils::Buffer& operator+=(
    torrentsync::utils::Buffer&,
    const std::string&);

