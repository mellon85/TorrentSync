#pragma once

#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

namespace torrentsync
{
namespace dht
{
namespace message
{

class BEncodeException : public std::exception
{
public:
    BEncodeException( const std::string reason )
        : std::exception(reason) {}
}

//! Decoder for a BEncode message decoder
class BEncodeDecoder
{
public:

    //! Constructor
    BEncodeDecoder( std::istream &stream )
        : stream(stream) {}

    //! Destructor
    virtual ~BEncodeDecoder();

    //! Parse the message from the istream
    //! @param istream stream parser
    void parseMessage();

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
    std::string readElement() const;

    std::vector<bool> structureStack;

    std::istream &stream

    enum {
        DICTIONARY = false,
        LIST = true
    };

};

} // torrentsync
} // dht
} // message

