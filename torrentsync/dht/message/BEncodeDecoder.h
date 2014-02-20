#pragma once

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <exception>

#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

typedef std::map<std::string,torrentsync::utils::Buffer> DataMap;

class BEncodeException : public std::exception
{
public:
    BEncodeException( const std::string& reason ) throw ();
    BEncodeException( const char* reason ) throw ();

    virtual ~BEncodeException() throw () {}

    virtual const char* what() const throw()
    {
        return reason.c_str();
    }

    const std::string reason;
};

//! Decoder for a BEncode message decoder
class BEncodeDecoder
{
public:

    //! Constructor
    BEncodeDecoder() {}

    //! Destructor
    virtual ~BEncodeDecoder() {}

    //! Parse the message from the istream
    //! @param stream stream parser
    void parseMessage( std::istream &stream );

    const DataMap& getData() const { return data; }

private:
    std::string readElement( std::istream& stream );
    torrentsync::utils::Buffer readValue( std::istream& stream );

    typedef std::pair<std::string,bool> structureStackE;
    std::vector<structureStackE> structureStack;

    DataMap data;

    int listCounter;

    std::string last_token;

    enum {
        DICTIONARY = false,
        LIST = true
    };

};


BEncodeException::BEncodeException( const std::string& str ) throw () : reason(str) {}
BEncodeException::BEncodeException( const char* str ) throw () : reason(str) {}

} // torrentsync
} // dht
} // message

