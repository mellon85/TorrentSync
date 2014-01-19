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

    void addElement( const std::string& v )
    {
        result << v.length();
        result << ':';
        result << v;
    }

    void addDictionaryElement( const std::string& k, const std::string& v )
    {
        // test key correctness with lexicographical_compare in an assert
        if (!lastKey.empty() && std::lexicographical_compare(k.begin(),k.end(),lastKey.begin(),lastKey.end()))
            throw std::logic_error("Violating lexicographic order constraint in dictionary");

        addElement(k);
        addElement(v);
        lastKey = k;
    }

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

    inline void startList()
    {
        result << "l";
    }

    inline void endList()
    {
        result << "e";
    }

    inline void startDictionary()
    {
        result << "d";
    }

    inline void endDictionary()
    {
        lastKey.clear();
        result << "e";
    }


    inline std::string value() const { return result.str(); }

private:
    std::string lastKey;
    std::stringstream result;

};

} // torrentsync
} // dht
} // message

