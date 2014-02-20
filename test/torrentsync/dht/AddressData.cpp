
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

#include <stdexcept>
#include <torrentsync/dht/AddressData.h>
#include <test/torrentsync/dht/CommonAddressTest.h>

namespace
{
class AddressData_fix : public torrentsync::dht::AddressData
{
};
};
 
BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_AddressData,AddressData_fix);

using namespace torrentsync::dht;

BOOST_AUTO_TEST_CASE(static_data_uppercase)
{
    const std::string data = "FFFFFFFFFFFFFFFF0000000000000001AAAAAAAA";
    BOOST_REQUIRE_NO_THROW(parse(data));
    BOOST_REQUIRE_EQUAL(0xFFFFFFFFFFFFFFFF, p1);
    BOOST_REQUIRE_EQUAL(0x0000000000000001, p2);
    BOOST_REQUIRE_EQUAL(0xAAAAAAAA, p3);
    BOOST_REQUIRE(boost::iequals(data,string()));
}

BOOST_AUTO_TEST_CASE(static_data_lowercase)
{
    const std::string data = "ffffffffffffffff0000000000000001aaaaaaaa";
    BOOST_REQUIRE_NO_THROW(parse(data));
    BOOST_REQUIRE_EQUAL(0xFFFFFFFFFFFFFFFF, p1);
    BOOST_REQUIRE_EQUAL(0x0000000000000001, p2);
    BOOST_REQUIRE_EQUAL(0xAAAAAAAA, p3);
    BOOST_REQUIRE(boost::iequals(data,string()));
}

BOOST_AUTO_TEST_CASE(static_data_mixedcase)
{
    const std::string data = "fFFFffffffffFFFF0000000000000001aaAAAAAa";
    BOOST_REQUIRE_NO_THROW(parse(data));
    BOOST_REQUIRE_EQUAL(0xFFFFFFFFFFFFFFFF, p1);
    BOOST_REQUIRE_EQUAL(0x0000000000000001, p2);
    BOOST_REQUIRE_EQUAL(0xAAAAAAAA, p3);
    BOOST_REQUIRE(boost::iequals(data,string()));
}

BOOST_AUTO_TEST_CASE(rand_data)
{
    for (int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        const std::string data = generateRandomAddress();
        BOOST_REQUIRE_NO_THROW(parse(data));
        BOOST_REQUIRE(boost::iequals(data,string()));
    }
}

BOOST_AUTO_TEST_CASE(too_short)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
	{
        std::string s;
        s.assign(rand()%40,'1');
        BOOST_REQUIRE_THROW(parse(s), std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(too_long)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::string s;
        s.assign(rand()%TEST_LOOP_COUNT+40+1,'1');
        BOOST_REQUIRE_THROW(parse(s), std::invalid_argument);
    }
}

BOOST_AUTO_TEST_CASE(comparing_fixed1)
{
	AddressData a1("0F00000000000000000000000000000000000000");
	AddressData a2("FF00000000000000000000000000000000000000");
	AddressData a3("FFFF000000000000000000000000000000000000");
    
	BOOST_REQUIRE_EQUAL(a2 >  a1, true);
	BOOST_REQUIRE_EQUAL(a2 >= a1, true);
	BOOST_REQUIRE_EQUAL(a2 <  a1, false);
	BOOST_REQUIRE_EQUAL(a2 <= a1, false);
	BOOST_REQUIRE_EQUAL(a2 == a1, false);
	
	BOOST_REQUIRE_EQUAL(a3 >  a2, true);
	BOOST_REQUIRE_EQUAL(a3 >= a2, true);
	BOOST_REQUIRE_EQUAL(a3 >  a1, true);
	BOOST_REQUIRE_EQUAL(a3 >= a1, true);
	BOOST_REQUIRE_EQUAL(a3 <  a2, false);
	BOOST_REQUIRE_EQUAL(a3 <= a2, false);
	BOOST_REQUIRE_EQUAL(a3 == a2, false);
	BOOST_REQUIRE_EQUAL(a3 <  a1, false);
	BOOST_REQUIRE_EQUAL(a3 <= a1, false);
	BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(comparing_fixed2)
{
	AddressData a1("0000000000000000000000000000000000000000");
    AddressData a2("0000000000000000000F00000000000000000000");
	AddressData a3("FFFF000000000000000000000000000000000000");
	
	BOOST_REQUIRE_EQUAL(a2 >  a1, true);
	BOOST_REQUIRE_EQUAL(a2 >= a1, true);
	BOOST_REQUIRE_EQUAL(a2 <  a1, false);
	BOOST_REQUIRE_EQUAL(a2 <= a1, false);
	BOOST_REQUIRE_EQUAL(a2 == a1, false);
	
	BOOST_REQUIRE_EQUAL(a3 >  a2, true);
	BOOST_REQUIRE_EQUAL(a3 >= a2, true);
	BOOST_REQUIRE_EQUAL(a3 >  a1, true);
	BOOST_REQUIRE_EQUAL(a3 >= a1, true);
	BOOST_REQUIRE_EQUAL(a3 <  a2, false);
	BOOST_REQUIRE_EQUAL(a3 <= a2, false);
	BOOST_REQUIRE_EQUAL(a3 == a2, false);
	BOOST_REQUIRE_EQUAL(a3 <  a1, false);
	BOOST_REQUIRE_EQUAL(a3 <= a1, false);
	BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(comparing_fixed3)
{
	AddressData a1("000F000000000000000000000000000000000000");
    AddressData a2("000F000000000000000F00000000000000000000");
	AddressData a3("FFFF000000000000000000000000000000000000");
    
	BOOST_REQUIRE_EQUAL(a2 >  a1, true);
	BOOST_REQUIRE_EQUAL(a2 >= a1, true);
	BOOST_REQUIRE_EQUAL(a2 <  a1, false);
	BOOST_REQUIRE_EQUAL(a2 <= a1, false);
	BOOST_REQUIRE_EQUAL(a2 == a1, false);
	
	BOOST_REQUIRE_EQUAL(a3 >  a2, true);
	BOOST_REQUIRE_EQUAL(a3 >= a2, true);
	BOOST_REQUIRE_EQUAL(a3 >  a1, true);
	BOOST_REQUIRE_EQUAL(a3 >= a1, true);
	BOOST_REQUIRE_EQUAL(a3 <  a2, false);
	BOOST_REQUIRE_EQUAL(a3 <= a2, false);
	BOOST_REQUIRE_EQUAL(a3 == a2, false);
	BOOST_REQUIRE_EQUAL(a3 <  a1, false);
	BOOST_REQUIRE_EQUAL(a3 <= a1, false);
	BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(comparing_fixed4)
{
	AddressData a1("000F000000000000000F00000000000000000000");
    AddressData a2("000F000000000000000F00000000000000F00000");
	AddressData a3("FFFF000000000000000000000000000000000000");
    
	BOOST_REQUIRE_EQUAL(a2 >  a1, true);
	BOOST_REQUIRE_EQUAL(a2 >= a1, true);
	BOOST_REQUIRE_EQUAL(a2 <  a1, false);
	BOOST_REQUIRE_EQUAL(a2 <= a1, false);
	BOOST_REQUIRE_EQUAL(a2 == a1, false);
	
	BOOST_REQUIRE_EQUAL(a3 >  a2, true);
	BOOST_REQUIRE_EQUAL(a3 >= a2, true);
	BOOST_REQUIRE_EQUAL(a3 >  a1, true);
	BOOST_REQUIRE_EQUAL(a3 >= a1, true);
	BOOST_REQUIRE_EQUAL(a3 <  a2, false);
	BOOST_REQUIRE_EQUAL(a3 <= a2, false);
	BOOST_REQUIRE_EQUAL(a3 == a2, false);
	BOOST_REQUIRE_EQUAL(a3 <  a1, false);
	BOOST_REQUIRE_EQUAL(a3 <= a1, false);
	BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(splitInHalf_ok)
{
	AddressData low = AddressData::minValue;
	AddressData high = AddressData::maxValue;
    
    MaybeBounds bounds = AddressData::splitInHalf(low,high);

    BOOST_REQUIRE(!!bounds);
	BOOST_REQUIRE_LT(low,bounds->first);
	BOOST_REQUIRE_LT(bounds->first,bounds->second);
	BOOST_REQUIRE_LT(bounds->second,high);
    
    BOOST_REQUIRE_EQUAL(bounds->first.string(),"7fffffffffffffffffffffffffffffffffffffff");
    BOOST_REQUIRE_EQUAL(bounds->second.string(),"8000000000000000000000000000000000000000");

    // sub bounds low-bounds.
    MaybeBounds bounds_low = AddressData::splitInHalf(low,bounds->first);

    BOOST_REQUIRE(!!bounds_low);
	BOOST_REQUIRE_LT(low,bounds_low->first);
	BOOST_REQUIRE_LT(bounds_low->first,bounds_low->second);
	BOOST_REQUIRE_LT(bounds_low->second,bounds->first);
    
    BOOST_REQUIRE_EQUAL(bounds_low->first.string(),"3fffffffffffffffffffffffffffffffffffffff");
    BOOST_REQUIRE_EQUAL(bounds_low->second.string(),"4000000000000000000000000000000000000000");

    // sub bounds high-bounds.
    MaybeBounds bounds_high = AddressData::splitInHalf(bounds->second,high);

    BOOST_REQUIRE(!!bounds_high);
	BOOST_REQUIRE_LT(bounds->second,bounds_high->first);
	BOOST_REQUIRE_LT(bounds_high->first,bounds_high->second);
	BOOST_REQUIRE_LT(bounds_high->second,high);
    
    BOOST_REQUIRE_EQUAL(bounds_high->first.string(),"bfffffffffffffffffffffffffffffffffffffff");
    BOOST_REQUIRE_EQUAL(bounds_high->second.string(),"c000000000000000000000000000000000000000");
}

BOOST_AUTO_TEST_CASE(bytestring_parsing)
{
    const std::string data = "1122334455667788aabbccddeeffgghhAABBCCDD";
    BOOST_REQUIRE_NO_THROW(parseByteString(data));
    BOOST_REQUIRE(boost::iequals(byteString(),data));
}

BOOST_AUTO_TEST_CASE(bytestring_parsing_special)
{
    const std::string data = "1122334455667788\tabb\ncdde\0ffgghhAABBCCDD";
    BOOST_REQUIRE_NO_THROW(parseByteString(data));
    BOOST_REQUIRE(boost::iequals(byteString(),data));
}

BOOST_AUTO_TEST_CASE(bytestring_errors)
{
    const std::string data = "112334455667788\tabb\ncdde\0ffgghhAABBCCDD";
    BOOST_REQUIRE_THROW(parseByteString(data), std::illegal_argument);

    const std::string data2 = "1122334455667788\tabb\ncdde\0ffgghhAABBCCDD1234567890";
    BOOST_REQUIRE_THROW(parseByteString(data2), std::illegal_argument);
}

BOOST_AUTO_TEST_SUITE_END();

