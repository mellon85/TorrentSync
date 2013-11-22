
#include <torrentsync/dht/AddressData.h>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/integer_traits.hpp>

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

class uint32hex
{
public:
	uint32_t value;
	inline uint32_t operator()() { return value; }
    inline friend std::istream& operator>>( std::istream& in, uint32hex& out )
    {
        in >> std::hex >> out.value;
    }
};

class uint64hex
{
public:
	uint64_t value;
	inline uint64_t operator()() { return value; }
    inline friend std::istream& operator>>( std::istream& in, uint64hex& out )
    {
        in >> std::hex >> out.value;
    }
};

template <class T>
inline bool isMax( T& v )
{
	return v == boost::integer_traits<T>::const_max;
}

};

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

AddressData& AddressData::increase()
{
	if (!isMax(p3))
		++p3;
	else
	{
		p3 = 0;
		if (!isMax(p2))
			++p2;
		else
		{
			p2 = 0;
			if (!isMax(p1))
				++p1;
			else
			{
				throw std::logic_error("Address is already at maximum value");
			}
		}
	}
	
	return *this;
}


const AddressData AddressData::minValue =
    AddressData("0000000000000000000000000000000000000000");
const AddressData AddressData::maxValue =
    AddressData("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

}; // dht
}; // torrentsync
