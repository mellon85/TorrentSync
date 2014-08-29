#pragma once

#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

using namespace torrentsync;

//! root is always a dictionary
class BEncodeEncoder
{
    public:

    BEncodeEncoder() : used_bytes(0), result(1024) {}

    void addElement(
        const std::string& v );

    void addDictionaryElement(
        const std::string& k,
        const std::string& v );
    
    void addElement(
        const utils::Buffer& v );

    void addDictionaryElement(
        const utils::Buffer& k,
        const utils::Buffer& v );

    void addDictionaryElement(
        const std::string& k,
        const utils::Buffer& v);
        
    template <class T>
    void addList( const T begin, const T end )
    {
        startList();
        while (begin != end)
        {
            addElement(*begin);
            ++begin;
        }
        endList();
    }

    void startList() { checkAndExpand(1); result[used_bytes++] = 'l'; }
    void endList() { checkAndExpand(1); result[used_bytes++] = 'e'; }
    void startDictionary() { checkAndExpand(1); result[used_bytes++] = 'd'; }
    void endDictionary() { lastKey.clear(); checkAndExpand(1); result[used_bytes++] = 'e'; }

    utils::Buffer value() const { result.resize(used_bytes); return result; } // copy to a buffer and pass it back..

private:

    template <class It>
    void addElement( It begin, const It end);

    template <class It, class It2>
    void addDictionaryElement(
        It  begin1, const It  end1,
        It2 begin2, const It2 end2 );

    size_t used_bytes;
    
    void checkAndExpand( const size_t add )
    {
        if( used_bytes+add > result.size() )
        {
            result.resize(((result.size()/1024)+1)*1024);
        }
    }

    mutable utils::Buffer result;

    utils::Buffer lastKey;
};


} // torrentsync
} // dht
} // message

