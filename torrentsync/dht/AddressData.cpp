
#include <sstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/integer_traits.hpp>
#include <boost/integer.hpp>

#include <torrentsync/dht/AddressData.h>

namespace
{
#if !defined(HAS_Cxx11)
bool isxdigit(const std::string& str)
{
    BOOST_FOREACH( const char c, str )
    {
        if (!std::isxdigit(c))
            return false;
    }
    return true;
}
#endif

template <size_t N>
class hexconverter
{
public:
    typedef typename boost::uint_t<N>::fast value_type;
    value_type value;
	inline value_type operator()() { return value; }
    inline friend std::istream& operator>>( std::istream& in, hexconverter<N>& out )
    {
        in >> std::hex >> out.value;
    }
};

typedef hexconverter<32> uint32hex;
typedef hexconverter<64> uint64hex;
}; // anonymous namespace

namespace torrentsync
{
namespace dht
{

const uint32_t AddressData::ADDRESS_STRING_LENGTH = 40;

AddressData::AddressData(const std::string& str)
{
	parse(str);
}

void AddressData::parse( const std::string& str )
{
    if (str.length() != 40 ||
#ifdef HAS_Cxx11
            !std::all_of(str.begin(),str.end(), ::isxdigit))
#else
            !isxdigit(str))
#endif
    {
        std::string msg = "Invalid argument: "; msg += str;
        throw std::invalid_argument(msg);
    }

	p1 = boost::lexical_cast<uint64hex>(str.substr(0,16))();
	p2 = boost::lexical_cast<uint64hex>(str.substr(16,16))();
	p3 = boost::lexical_cast<uint32hex>(str.substr(32))();
}

const std::string AddressData::string() const
{
    std::string ret;
    ret.reserve(40);
    std::stringstream ss;
    ss << std::hex;
    ss.width(16); ss.fill('0'); ss << p1;
    ss.width(16); ss.fill('0'); ss << p2;
    ss.width(8);  ss.fill('0'); ss << p3;
    ss >> ret;
    return ret;
}

const AddressData AddressData::minValue =
    AddressData("0000000000000000000000000000000000000000");
const AddressData AddressData::maxValue =
    AddressData("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

AddressData::MaybeBounds AddressData::splitInHalf(
    const AddressData& low,
    const AddressData& high)
{
    AddressData half_low,half_high;

    // a masks 1..10....0 where the 1s are the root of the tree mask
    // low & mask == high & mask
    AddressData mask;
    mask.p1 = ~(low.p1 ^ high.p1);
    mask.p2 = ~(low.p2 ^ high.p2);
    mask.p3 = ~(low.p3 ^ high.p3);

    // bit mask full set, can't split anymore
    if (~mask.p3 == 0 && ~mask.p2 == 0 && ~mask.p3 == 0)
        return MaybeBounds();

    // create the mask to set/unset the new bit to add to next addresses
    AddressData new_bit = mask;
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

}; // dht
}; // torrentsync
