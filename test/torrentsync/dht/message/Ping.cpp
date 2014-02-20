#include <boost/test/unit_test.hpp>
#include <torrentsync/dht/message/Ping.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_Ping);

using namespace torrentsync::dht::message;

#define TEST_FIELD(key,value) { \
    BOOST_REQUIRE(map.find(key) != map.end()); \
    BOOST_REQUIRE_EQUAL(map.find(key)->second,value);}

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    Ping p();
}

BOOST_AUTO_TEST_CASE(generation_1)
{
/*
    std::string out;
    std::string ret;

    std::string transaction = "aa";
    AddressData data("");

    ret = Ping::getMessage(
         transaction
        ,data.getByteString()
        ,out);

    BOOST_REQUIRE_EQUAL(ret,out);
    BOOST_REQUIRE_EQUAL("d1:ad2:id20:abcdefghij0123456789e1:q4:ping1:t2:aa1:y1:qe",out);
*/
}

BOOST_AUTO_TEST_SUITE_END();
