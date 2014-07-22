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

//! root is always a dictionary
class BEncodeEncoder
{
    public:

    BEncodeEncoder() : used_bytes(0), result(1024) {}

    void addElement(
        const torrentsync::utils::Buffer& v );

    void addDictionaryElement(
        const torrentsync::utils::Buffer& k,
        const torrentsync::utils::Buffer& v );

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

    torrentsync::utils::Buffer value() const { result.resize(used_bytes); return result; } // copy to a buffer and pass it back..

private:

    size_t used_bytes;
    
    void checkAndExpand( const size_t add )
    {
        if( used_bytes+add > result.size() )
        {
            result.resize(((result.size()/1024)+1)*1024);
        }
    }

    mutable torrentsync::utils::Buffer result;

    torrentsync::utils::Buffer lastKey;
};


} // torrentsync
} // dht
} // message

