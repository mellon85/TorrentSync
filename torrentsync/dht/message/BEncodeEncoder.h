/*
 * BEncodedEncoder.h
 */

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

    BEncodeEncoder() {}

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

    inline void startList() { result << "l"; }
    inline void endList() { result << "e"; }
    inline void startDictionary() { result << "d"; }
    inline void endDictionary() { lastKey.clear(); result << "e"; }
    inline std::string value() const { return result.str(); }

private:
    torrentsync::utils::Buffer lastKey;
    std::stringstream result;

};

void BEncodeEncoder::addDictionaryElement(
    const torrentsync::utils::Buffer& k,
    const torrentsync::utils::Buffer& v )
{
    // test key correctness with lexicographical_compare in an assert
    if (!lastKey.empty() && std::lexicographical_compare(k.begin(),k.end(),lastKey.begin(),lastKey.end()))
        throw std::logic_error("Violating lexicographic order constraint in dictionary");

    addElement(k);
    addElement(v);
    lastKey = k;
}

void BEncodeEncoder::addElement( const torrentsync::utils::Buffer& v )
{
    assert(!v.empty());
    result << v.size();
    result << ':';
    result.write(v.get(),v.size());
}

} // torrentsync
} // dht
} // message

