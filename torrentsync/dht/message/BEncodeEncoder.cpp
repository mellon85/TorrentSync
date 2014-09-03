#include <torrentsync/dht/message/BEncodeEncoder.h>

#include <boost/lexical_cast.hpp>

#include <cassert>

namespace torrentsync
{
namespace dht
{
namespace message
{

void BEncodeEncoder::addDictionaryElement(
    const std::string& k,
    const std::string& v )
{
    addDictionaryElement(k.cbegin(),k.cend(),
        v.cbegin(),v.cend());
}

void BEncodeEncoder::addDictionaryElement(
    const std::string& k,
    const utils::Buffer& v )
{
    addDictionaryElement(k.cbegin(),k.cend(),
        v.cbegin(),v.cend());
}

void BEncodeEncoder::addDictionaryElement(
    const utils::Buffer& k,
    const utils::Buffer& v )
{
    addDictionaryElement(k.cbegin(),k.cend(),
        v.cbegin(),v.cend());
}

void BEncodeEncoder::addElement( const std::string& s )
{
    addElement(s.cbegin(),s.cend());
}

void BEncodeEncoder::addElement( const utils::Buffer& v )
{
    addElement(v.cbegin(),v.cend());
}

void BEncodeEncoder::addInteger( uint64_t v )
{
    const size_t digits = ceil(log10(v));

    checkAndExpand(digits+2);
    *(result.begin()+used_bytes++) = 'i';

    const auto str = boost::lexical_cast<std::string>(v);
    std::copy(str.cbegin(),str.cend(),result.begin()+used_bytes);
    used_bytes += digits;
    *(result.begin()+used_bytes++) = 'e';
}

template <class It>
void BEncodeEncoder::addElement(
    It begin,
    const It end)
{
    assert(begin != end);

    auto length = std::distance(begin,end);
    auto length_string = boost::lexical_cast<std::string>(length);

    checkAndExpand(length_string.size()+1+1+length);

    std::copy(
        length_string.begin(),
        length_string.end(),
        result.begin()+used_bytes);

    used_bytes += length_string.size();
    result[used_bytes++] = ':';
    std::copy(begin,end,
        result.begin()+used_bytes);
    used_bytes += length;
}

template <class It, class It2>
void BEncodeEncoder::addDictionaryElement(
    It  begin1, const It  end1,
    It2 begin2, const It2 end2 )
{
    // test key correctness with lexicographical_compare in an assert
    if (!lastKey.empty() && std::lexicographical_compare(begin1,end1,lastKey.cbegin(),lastKey.cend()))
        throw std::logic_error("Violating lexicographic order constraint in dictionary");

    addElement(begin1,end1);
    addElement(begin2,end2);
    
    lastKey = utils::Buffer(begin1,end1);
}

void BEncodeEncoder::checkAndExpand( const size_t add )
{
    if( used_bytes+add > result.size() )
    {
        result.resize(((result.size()/1024)+1)*1024);
    }
}

} // torrentsync
} // dht
} // message
