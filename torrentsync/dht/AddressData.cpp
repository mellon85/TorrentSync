
#include <torrentsync/dht/AddressData.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/integer_traits.hpp>
#include <boost/integer.hpp>

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

}; // dht
}; // torrentsync
