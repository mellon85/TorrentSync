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

    template <class T>
    void addElement( const T v );

    template <class A, class B>
    void addDictionaryElement( const A k, const B v );

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
    std::string lastKey;
    std::stringstream result;

    void addDictionaryElementImpl( const std::string& k, const std::string& v );
    void addElementImpl( const std::string& v );
};

namespace {
template <class T> inline std::string make_string( T v )
{
    return std::string(v);
}
template <> inline std::string make_string( char v )
{
    std::string s; s.append(1,v);
    return s;
}
template <> inline std::string make_string( std::string& v )
{
    return v;
}
};

template <class T>
inline void BEncodeEncoder::addElement( const T v ) {
    addElementImpl(make_string(v));
}

template <class A, class B>
inline void BEncodeEncoder::addDictionaryElement( const A k, const B v )
{
    addDictionaryElementImpl(make_string(k), make_string(v));
}

void BEncodeEncoder::addDictionaryElementImpl( const std::string& k, const std::string& v )
{
    // test key correctness with lexicographical_compare in an assert
    if (!lastKey.empty() && std::lexicographical_compare(k.begin(),k.end(),lastKey.begin(),lastKey.end()))
        throw std::logic_error("Violating lexicographic order constraint in dictionary");

    addElement(k);
    addElement(v);
    lastKey = k;
}

void BEncodeEncoder::addElementImpl( const std::string& v )
{
    result << v.length();
    result << ':';
    result << v;
}

} // torrentsync
} // dht
} // message

