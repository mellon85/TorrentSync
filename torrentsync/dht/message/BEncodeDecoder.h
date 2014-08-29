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

typedef std::runtime_error BEncodeException;

//! Decoder for a bencoded message
class BEncodeDecoder
{
public:
    //! Constructor
    BEncodeDecoder() {}

    //! Destructor
    ~BEncodeDecoder() {}

    /*! Parse the message from a istream
     *  @param stream stream parser
     */
    void parseMessage( std::istream &stream );

    const DataMap& getData() const noexcept { return data; }

    boost::optional<
        torrentsync::utils::Buffer> find( const std::string& key ) const;

private:
    //! parse a single element from the stream
    std::string readElement( std::istream& stream );

    //! parse a single value from the stream
    torrentsync::utils::Buffer readValue( std::istream& stream );

    //! Stack entry
    typedef std::pair<std::string,bool> structureStackE;

    //! Stack to use while parsing
    std::vector<structureStackE> structureStack;

    //! Container of all the parsed data
    DataMap data;

    typedef enum {
        DICTIONARY = 0,
        LIST
    } structure_t;

};

} // torrentsync
} // dht
} // message

