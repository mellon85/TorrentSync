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

    inline void startList() { checkAndExpand(1); result[used_bytes++] = 'l'; }
    inline void endList() { checkAndExpand(1); result[used_bytes++] = 'e'; }
    inline void startDictionary() { checkAndExpand(1); result[used_bytes++] = 'd'; }
    inline void endDictionary() { lastKey.clear(); checkAndExpand(1); result[used_bytes++] = 'e'; }

    inline torrentsync::utils::Buffer value() const { result.resize(used_bytes); return result; } // copy to a buffer and pass it back..

private:

    size_t used_bytes;
    
    inline void checkAndExpand( const size_t add )
    {
        if( used_bytes+add > result.size() )
        {
            result.resize(((result.size()/1024)+1 *1024));
        }
    }

    mutable torrentsync::utils::Buffer result;

    torrentsync::utils::Buffer lastKey;
};

void BEncodeEncoder::addDictionaryElement(
    const torrentsync::utils::Buffer& k,
    const torrentsync::utils::Buffer& v )
{
    // test key correctness with lexicographical_compare in an assert
    if (!lastKey.empty() && std::lexicographical_compare(k.cbegin(),k.cend(),lastKey.cbegin(),lastKey.cend()))
        throw std::logic_error("Violating lexicographic order constraint in dictionary");

    addElement(k);
    addElement(v);
    lastKey = k;
}

void BEncodeEncoder::addElement( const torrentsync::utils::Buffer& v )
{
    assert(!v.empty());

    char length_as_string[16];
    const size_t len = snprintf(length_as_string,64,"%lu",v.size());
    assert(len > 0); // can't put more than 16 digits of data..

    checkAndExpand(len+1+1+v.size());

    result.copy(used_bytes, static_cast<const char*>(length_as_string),len);
    used_bytes += len;
    result[used_bytes++] = ':';
    result.copy(used_bytes,v.get(),v.size());
    used_bytes += v.size();
}

} // torrentsync
} // dht
} // message

