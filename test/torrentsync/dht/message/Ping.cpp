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
    ret = Ping::getMessage(transaction
                          ,data));

    BOOST_REQUIRE(
        torrentsync::utils::Buffer("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe") == ret);
}

BOOST_AUTO_TEST_CASE(parse)
{
    torrentsync::utils::Buffer b("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");
    std::stringstream s;
    s.write(b.get(),b.size());

    
    auto m = torrentsync::dht::message::Message::parseMessage(s);
    BOOST_REQUIRE(!!m);
    BOOST_REQUIRE_EQUAL(m->getType(), Type::Query);
    BOOST_REQUIRE_EQUAL(m->getMessageType(), Messages::Ping);

/*  TODO make this compile/work
    auto p = dynamic_pointer_cast<torrentsync::dht::message::Ping*>(m.get());

    BOOST_REQUIRE_EQUAL(p->getID(), "GGGGGGGGHHHHHHHHIIII");
    BOOST_REQUIRE_EQUAL(p->getToken(), "aa");
*/

}

BOOST_AUTO_TEST_SUITE_END()
