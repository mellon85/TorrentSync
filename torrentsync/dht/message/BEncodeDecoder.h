#pragma once

#include <list>
#include <unordered_map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <boost/optional.hpp>

#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

typedef std::unordered_map<std::string,torrentsync::utils::Buffer> DataMap;

class BEncodeException : public std::runtime_error
{
public:
    BEncodeException( const std::string& reason ) : std::runtime_error(reason) {}
};

//! Decoder for a BEncode message decoder
class BEncodeDecoder
{
public:

    //! Constructor
    BEncodeDecoder() {}

    //! Destructor
    virtual ~BEncodeDecoder() {}

    //! Parse the message from a istream
    //! @param stream stream parser
    void parseMessage( std::istream &stream );

    const DataMap& getData() const noexcept { return data; }

    boost::optional<
        torrentsync::utils::Buffer> find( const std::string& key ) const;

private:
    std::string readElement( std::istream& stream );
    torrentsync::utils::Buffer readValue( std::istream& stream );

    typedef std::pair<std::string,bool> structureStackE;
    std::vector<structureStackE> structureStack;

    DataMap data;

    enum {
        DICTIONARY = false,
        LIST = true
    };

};

} // torrentsync
} // dht
} // message

