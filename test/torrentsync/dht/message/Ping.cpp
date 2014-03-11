#include <boost/test/unit_test.hpp>
#include <torrentsync/dht/message/Ping.h>
#include <torrentsync/dht/NodeData.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_Ping);

using namespace torrentsync::dht::message;
using namespace torrentsync::dht;

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    //torrentsync::dht::message::Ping p;
}

BOOST_AUTO_TEST_CASE(generation_1)
{
    std::string out;
    std::string ret;

    std::string transaction = "aa";
    const char bytestring[20] = 
        {71,71,71,71,71,71,71,71,72,72,72,72,72,72,72,72,73,73,73,73};
    torrentsync::utils::Buffer b = putInBuffer(bytestring);
    NodeData data;
    data.read(b.cbegin(),b.cend());

    BOOST_REQUIRE_NO_THROW(
    ret = Ping::getMessage(
         transaction
        ,data
        ,out));

    BOOST_REQUIRE_EQUAL(ret,out);
    BOOST_REQUIRE_EQUAL("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe",out);
}

BOOST_AUTO_TEST_SUITE_END();
