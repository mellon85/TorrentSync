
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

BOOST_AUTO_TEST_SUITE_END();

