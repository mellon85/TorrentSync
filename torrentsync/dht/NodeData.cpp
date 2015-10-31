
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <random>

#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Distance.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/utils/log/Logger.h>

#include <netinet/in.h>

namespace torrentsync
{
namespace dht
{

const size_t NodeData::ADDRESS_STRING_LENGTH = 40;
const size_t NodeData::addressDataLength     = 20;

static std::random_device random;

NodeData::NodeData(const utils::Buffer& buff)
{
    if ( buff.size() != addressDataLength)
    {
        std::stringstream msg;
        msg << "NodeData expects " << addressDataLength << " byte as address";
        throw std::invalid_argument(msg.str());
    }

    const uint32_t * const data = reinterpret_cast<const uint32_t*>(&buff[0]);

    p1 = htonl(data[0]);
    p1 <<= 32;
    p1 |= htonl(data[1]);
    p2 =  htonl(data[2]);
    p2 <<= 32;
    p2 |= htonl(data[3]);
    p3 =  htonl(data[4]);
}

NodeData::~NodeData()
{
}

Distance NodeData::operator^( const NodeData& addr ) const noexcept
{
    Distance ret;
    ret.p1 = p1 ^ addr.p1;
    ret.p2 = p2 ^ addr.p2;
    ret.p3 = p3 ^ addr.p3;
    return ret;
}

const std::string NodeData::string() const
{
    std::ostringstream ss;

    ss << std::hex;
    ss.width(16); ss.fill('0'); ss << p1;
    ss.width(16); ss.fill('0'); ss << p2;
    ss.width(8);  ss.fill('0'); ss << p3;
    return ss.str();
}

const NodeData NodeData::minValue(
    utils::Buffer({0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}));
const NodeData NodeData::maxValue(
    utils::Buffer({0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}));

MaybeBounds NodeData::splitInHalf(
    const NodeData& low,
    const NodeData& high)
{
    NodeData half_low,half_high;

    // a masks 1..10....0 where the 1s are the root of the tree mask
    // low & mask == high & mask
    NodeData mask;
    mask.p1 = ~(low.p1 ^ high.p1);
    mask.p2 = ~(low.p2 ^ high.p2);
    mask.p3 = ~(low.p3 ^ high.p3);

    // bit mask full set, can't split anymore
    if (~mask.p3 == 0 && ~mask.p2 == 0 && ~mask.p3 == 0)
        return MaybeBounds();

    // create the mask to set/unset the new bit to add to next addresses
    NodeData new_bit = mask;
    const bool p2_first_bit = new_bit.p2 & 1;
    const bool p1_first_bit = new_bit.p1 & 1;
    new_bit.p1 >>= 1;
    new_bit.p1 |= 0x8000000000000000;
    new_bit.p2 >>= p1_first_bit;
    new_bit.p2 |= 0x8000000000000000 * (p1_first_bit); // set bit for carryover
    new_bit.p3 >>= p2_first_bit;
    new_bit.p3 |= 0x80000000 * (p2_first_bit); // set bit for carryover
    new_bit.p1 ^= mask.p1;
    new_bit.p2 ^= mask.p2;
    new_bit.p3 ^= mask.p3;

    // assert (only one bit set in all p1, p2, p3

    // creating half_low (the high of the lower pair)
    half_low = low;
    // also all the bits on the right of the bit are 0s, and must be 1s
    half_low.p1 |= ~(mask.p1 | new_bit.p1);
    half_low.p2 |= ~(mask.p2 | new_bit.p2);
    half_low.p3 |= ~(mask.p3 | new_bit.p3);

    // creating half_high (the low of the higher pair)
    half_high = high;
    // also all the bits on the right of the bit are 1s, and must be 0s
    half_high.p1 &= mask.p1 | new_bit.p1;
    half_high.p2 &= mask.p2 | new_bit.p2;
    half_high.p3 &= mask.p3 | new_bit.p3;

    return MaybeBounds(Bounds(half_low,half_high));
}

const NodeData NodeData::getRandom()
{
    static std::uniform_int_distribution<uint32_t> dist;
    static std::uniform_int_distribution<uint64_t> dist64;
    NodeData data;
    using namespace torrentsync::utils;

    data.p1 = dist64(random);
    data.p2 = dist64(random);
    data.p3 = dist(random);

    return data;
}

void NodeData::read(
    utils::Buffer::const_iterator begin,
    const utils::Buffer::const_iterator end  )
{
    if ( end < begin || (static_cast<size_t>((end-begin)) < addressDataLength) )
    {
        LOG(ERROR,"NodeData - parseNode: not enough data to parse. Expected " << addressDataLength << ", found: " << (end-begin) );
        throw std::invalid_argument("Wrong amount of data to parse");
    }

    p1 = 0;
    p2 = 0;
    p3 = 0;

    for ( int i = 0; i <= 7; ++i )
    {
        p1 <<= 8;
        p1 |= *(begin++);
    }

    for ( int i = 8; i <= 15; ++i )
    {
        p2 <<= 8;
        p2 |= *(begin++);
    }

    for ( int i = 16; i <= 19; ++i )
    {
        p3 <<= 8;
        p3 |= *(begin++);
    }
}

torrentsync::utils::Buffer NodeData::write() const
{
    torrentsync::utils::Buffer buff;
    buff.reserve(20);

    buff.push_back(p1 >> 56 & 0xFF);
    buff.push_back(p1 >> 48 & 0xFF);
    buff.push_back(p1 >> 40 & 0xFF);
    buff.push_back(p1 >> 32 & 0xFF);
    buff.push_back(p1 >> 24 & 0xFF);
    buff.push_back(p1 >> 16 & 0xFF);
    buff.push_back(p1 >>  8 & 0xFF);
    buff.push_back(p1 >>  0 & 0xFF);
    buff.push_back(p2 >> 56 & 0xFF);
    buff.push_back(p2 >> 48 & 0xFF);
    buff.push_back(p2 >> 40 & 0xFF);
    buff.push_back(p2 >> 32 & 0xFF);
    buff.push_back(p2 >> 24 & 0xFF);
    buff.push_back(p2 >> 16 & 0xFF);
    buff.push_back(p2 >>  8 & 0xFF);
    buff.push_back(p2 >>  0 & 0xFF);
    buff.push_back(p3 >> 24 & 0xFF);
    buff.push_back(p3 >> 16 & 0xFF);
    buff.push_back(p3 >>  8 & 0xFF);
    buff.push_back(p3 >>  0 & 0xFF);

    return buff;
}

std::ostream& operator<<( std::ostream& out, const NodeData& data )
{
    out << data.string();
    return out;
}

}; // dht
}; // torrentsync
