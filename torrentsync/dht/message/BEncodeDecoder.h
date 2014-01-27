#pragma once

#include <list>
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

protected:

    //! called when a dictionary starts
    virtual void onDictionaryStart()=0;
    
    //! called when a dictionary ends
    virtual void onDictionaryEnd()=0;

    //! called when a list starts
    virtual void onListStart()=0;
    
    //! called when a list ends
    virtual void onListEnd()=0;

    //! an element has been read
    //! @param element the element content
    virtual void onElement(
            const std::string& element)=0;

    //! an element has been read
    //! @param key the key content
    //! @param element the element content
    virtual void onElement(
            const std::string& key,
            const std::string& element)=0;

private:
    std::string readElement( std::istream& stream );

    std::vector<bool> structureStack;

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

