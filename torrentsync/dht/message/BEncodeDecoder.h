#pragma once

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <exception>

namespace torrentsync
{
namespace dht
{
namespace message
{

class BEncodeException : public std::exception
{
public:
    inline BEncodeException( const std::string& reason ) throw ();
    inline BEncodeException( const char* reason ) throw ();

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

    const std::map<std::string,std::string>& getData() const { return data; }

private:
    std::string readElement( std::istream& stream );

    typedef std::pair<std::string,bool> structureStackE;
    std::vector<structureStackE> structureStack;

    std::map<std::string,std::string> data;

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

