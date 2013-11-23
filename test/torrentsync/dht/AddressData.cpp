
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

BOOST_AUTO_TEST_CASE(increase)
{
    AddressData a1(generateRandomAddress());
	AddressData a2 = a1;
	
	a2.increase();
	BOOST_REQUIRE(a1 < a2);
	BOOST_REQUIRE(a1 <= a2);
	
	a1.increase();
	BOOST_REQUIRE(a1 == a2);
	BOOST_REQUIRE(a1 <= a2);
	
	a1.increase();
	BOOST_REQUIRE(a1 > a2);
	BOOST_REQUIRE(a1 >= a2);

	a2.increase();
	BOOST_REQUIRE(a1 == a2);
	BOOST_REQUIRE(a1 <= a2);
}

BOOST_AUTO_TEST_CASE(increase_carryover)
{
    AddressData a("0000000000000000000000000000000000000000"); // to have some space
	
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000001");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000002");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000003");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000004");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000005");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000006");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000007");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000008");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000009");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"000000000000000000000000000000000000000a");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"000000000000000000000000000000000000000b");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"000000000000000000000000000000000000000c");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"000000000000000000000000000000000000000d");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"000000000000000000000000000000000000000e");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"000000000000000000000000000000000000000f");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000000000010");
	
	
	a = AddressData("00000000000000000000000000000000FFFFFFFF");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000100000000");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000000000000000000100000001");
	
	a = AddressData("0000000000000000FFFFFFFFFFFFFFFF00000000");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000ffffffffffffffff00000001");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000000ffffffffffffffff00000002");
	
	a = AddressData("0000000000000000FFFFFFFFFFFFFFFFFFFFFFFF");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000001000000000000000000000000");
	a.increase();
	BOOST_REQUIRE_EQUAL(a.string(),"0000000000000001000000000000000000000001");
	
	a = AddressData("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
	BOOST_CHECK_THROW(a.increase(),std::logic_error);
}

BOOST_AUTO_TEST_CASE(increase_random)
{
    AddressData a1(generateRandomAddress("0000")); // to have some space
	AddressData a2 = a1;
	
	const int a2_1 = rand()%0xFF;
	for (int i = 0; i < a2_1; ++i)
	{
		a2.increase();
	}
	BOOST_REQUIRE(a1 < a2);
	BOOST_REQUIRE(a1 <= a2);
	
	const int a1_1 = rand()%a2_1;
	for (int i = 0; i < a1_1; ++i)
	{
		a1.increase();
	}
	BOOST_REQUIRE(a1 < a2);
	BOOST_REQUIRE(a1 <= a2);
	
	const int a1_2 = a2_1;
	for (int i = 0; i < a1_2; ++i)
	{
		a1.increase();
	}
	BOOST_REQUIRE(a1 >= a2);
    a1.increase();
	BOOST_REQUIRE(a1 > a2);
	BOOST_REQUIRE(a1 >= a2);
}

BOOST_AUTO_TEST_SUITE_END();

