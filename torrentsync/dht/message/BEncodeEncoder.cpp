#include <torrentsync/dht/message/BEncodeEncoder.h>

namespace torrentsync
{
namespace dht
{
namespace message
{


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

    assert(result.size()+used_bytes >= len); 

    std::copy(length_as_string,length_as_string+len,result.begin()+used_bytes);

    used_bytes += len;
    result[used_bytes++] = ':';
    std::copy(v.cbegin(),v.cend(),result.begin()+used_bytes);
    used_bytes += v.size();
}

} // torrentsync
} // dht
} // message
