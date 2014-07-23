#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/Ping.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>


BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_Ping);

using namespace torrentsync::dht::message;

BOOST_AUTO_TEST_CASE(generation_1)
{
    torrentsync::utils::Buffer ret;

    std::string transaction = "aa";
    const char bytestring[20] = {71,71,71,71,71,71,71,71,72,72,72,72,72,72,72,72,73,73,73,73};
    torrentsync::utils::Buffer b = putInBuffer(bytestring);
    torrentsync::dht::NodeData data;
    data.read(b.cbegin(),b.cend());

    BOOST_REQUIRE_NO_THROW(
        ret = Ping::getQuery(transaction,data)); 
    BOOST_REQUIRE_EQUAL(
        torrentsync::utils::Buffer("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe"),ret);
}

BOOST_AUTO_TEST_CASE(parse)
{
    torrentsync::utils::Buffer b("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");

    auto m = torrentsync::dht::message::Message::parseMessage(b);
    BOOST_REQUIRE(!!m);
    BOOST_REQUIRE_EQUAL(m->getType(), Type::Query);
    BOOST_REQUIRE_EQUAL(m->getMessageType(), Messages::Ping);

    auto p = dynamic_cast<torrentsync::dht::message::Ping*>(m.get());
    BOOST_REQUIRE(p);

    BOOST_REQUIRE_EQUAL(p->getID(),"GGGGGGGGHHHHHHHHIIII");
    BOOST_REQUIRE_EQUAL(p->getTransactionID(),"aa");
}

BOOST_AUTO_TEST_CASE(parseBinary)
{
    torrentsync::utils::Buffer b("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");
    b[15] = '\t';
    b[18] = '\0';

    auto m = torrentsync::dht::message::Message::parseMessage(b);
    BOOST_REQUIRE(!!m);
    BOOST_REQUIRE_EQUAL(m->getType(), Type::Query);
    BOOST_REQUIRE_EQUAL(m->getMessageType(), Messages::Ping);

    auto p = dynamic_cast<torrentsync::dht::message::Ping*>(m.get());
    BOOST_REQUIRE(p);

    torrentsync::utils::Buffer id("GGGGGGGGHHHHHHHHIIII");
    id[3] = '\t';
    id[6] = '\0';

    BOOST_REQUIRE_EQUAL(p->getID(),id);
    BOOST_REQUIRE_EQUAL(p->getTransactionID(),"aa");
}

BOOST_AUTO_TEST_CASE(parseRandom)
{
    for ( size_t i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        //! set address to 0s and then copy something inside
        torrentsync::utils::Buffer b("d1:ad2:id20:00000000000000000000e1:q4:ping1:t2:aa1:y1:qe");

        auto addr = torrentsync::dht::NodeData::getRandom();
        auto ab = addr.write();
        std::copy(ab.cbegin(),ab.cend(),b.begin()+12);

        auto m = torrentsync::dht::message::Message::parseMessage(b);
        BOOST_REQUIRE(!!m);
        BOOST_REQUIRE_EQUAL(m->getType(), Type::Query);
        BOOST_REQUIRE_EQUAL(m->getMessageType(), Messages::Ping);

        auto p = dynamic_cast<torrentsync::dht::message::Ping*>(m.get());
        BOOST_REQUIRE(p);

        BOOST_REQUIRE_EQUAL(p->getID(),ab);
        BOOST_REQUIRE_EQUAL(p->getTransactionID(),"aa");
    }
}

BOOST_AUTO_TEST_SUITE_END()
