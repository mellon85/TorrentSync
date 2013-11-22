
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

#include <torrentsync/dht/Address.h>
#include <test/torrentsync/dht/CommonAddressTest.h>

namespace
{
class AddressFixture : public torrentsync::dht::Address
{
public:
	AddressFixture() {}
};
};

BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_Address,AddressFixture);

using namespace torrentsync::dht;

BOOST_AUTO_TEST_CASE(distance_0)
{
    const std::string data = "FFFFFFFFFFFFFFFF0000000000000001AAAAAAAA";

    Address *addr1;
    BOOST_REQUIRE_NO_THROW(addr1 = new Address(data));
    Address *addr2;
    BOOST_REQUIRE_NO_THROW(addr2 = new Address(data));

    BOOST_REQUIRE(boost::iequals(data,addr1->string()));
    BOOST_REQUIRE(boost::iequals(data,addr2->string()));

    const Distance dist = *addr1 ^ *addr2;
    BOOST_REQUIRE_EQUAL(dist.string(),"0000000000000000000000000000000000000000");
}

BOOST_AUTO_TEST_CASE(distance_some_static)
{
    const std::string data1 = "FFFFFFFFFFFFFFFF0000000000000001AAAAAAAA";
    const std::string data2 = "FFFFFFFFFFFFFFFF0001100000000001AAAAAAAA";

    Address *addr1;
    BOOST_REQUIRE_NO_THROW(addr1 = new Address(data1));
    Address *addr2;
    BOOST_REQUIRE_NO_THROW(addr2 = new Address(data2));

    BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
    BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

    const Distance dist = *addr1 ^ *addr2;
    BOOST_REQUIRE_EQUAL(dist.string(),"0000000000000000000110000000000000000000");
    BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
    BOOST_REQUIRE(boost::iequals(data2,addr2->string()));
}

BOOST_AUTO_TEST_CASE(distance_random)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i)
    {
        std::string data1 = generateRandomAddress();
        const std::string data2 = data1;

        for ( unsigned int numOfDiff = rand()%40+1;
                numOfDiff > 0; --numOfDiff)
        {
            const int n = rand() % 16;
            data1[rand()%data1.length()] = static_cast<char>(n < 10 ? n+48 : (n%6)+97);
        }

        Address *addr1;
        BOOST_REQUIRE_NO_THROW(addr1 = new Address(data1));
        Address *addr2;
        BOOST_REQUIRE_NO_THROW(addr2 = new Address(data2));

        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        const Distance dist = *addr1 ^ *addr2;
        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        Address *addr3;
        BOOST_REQUIRE_NO_THROW(addr3 = new Address(dist.string()));
        const Distance dist1 = *addr3 ^ *addr2;
        const Distance dist2 = *addr3 ^ *addr1;

        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        BOOST_REQUIRE_EQUAL(dist1.string(),addr1->string());
        BOOST_REQUIRE_EQUAL(dist2.string(),addr2->string());
    }
}

BOOST_AUTO_TEST_CASE(is_Good_Bad_Questionale)
{
    setGood();
    BOOST_REQUIRE_EQUAL(true,isGood());
    BOOST_REQUIRE_EQUAL(isBad(),false);
    BOOST_REQUIRE_EQUAL(isQuestionable(),false);

    last_time_good = last_time_good - good_interval;
    BOOST_REQUIRE_EQUAL(last_unanswered_queries,0);

    BOOST_REQUIRE_EQUAL(isGood(),false);
    BOOST_REQUIRE_EQUAL(isQuestionable(),true);
    BOOST_REQUIRE_EQUAL(isBad(),false);

    last_unanswered_queries = allowed_unanswered_queries+1;

    BOOST_REQUIRE_EQUAL(isGood(),false);
    BOOST_REQUIRE_EQUAL(isQuestionable(),false);
    BOOST_REQUIRE_EQUAL(isBad(),true);
}	

BOOST_AUTO_TEST_SUITE_END();

