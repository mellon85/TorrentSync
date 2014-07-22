#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/FindNode.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>


BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_FindNode);

using namespace torrentsync::dht::message;
using namespace torrentsync::utils;

BOOST_AUTO_TEST_CASE(generation_1)
{
    torrentsync::utils::Buffer ret;

    std::string transaction = "aa";
    const char bytestring[20] = {'a','b','c','d','e','f','g','h','i','j','0','1','2','3','4','5','6','7','8','9'};
    const char bytestring_target[20] = {'m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4','5','6'};
    torrentsync::utils::Buffer b = putInBuffer(bytestring);
    torrentsync::utils::Buffer tb = putInBuffer(bytestring_target);
    torrentsync::dht::NodeData data, target;
    data.read(b.cbegin(),b.cend());
    target.read(tb.cbegin(),tb.cend());

    BOOST_REQUIRE_NO_THROW(
        ret = FindNode::getMessage(transaction,data,target)); 
    BOOST_REQUIRE_EQUAL(
        torrentsync::utils::Buffer("d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe"),ret);
}

BOOST_AUTO_TEST_CASE(parse)
{
    torrentsync::utils::Buffer b("d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe");

    auto m = torrentsync::dht::message::Message::parseMessage(b);
    BOOST_REQUIRE(!!m);
    BOOST_REQUIRE_EQUAL(m->getType(), Type::Query);
    BOOST_REQUIRE_EQUAL(m->getMessageType(), Messages::FindNode);

    auto p = dynamic_cast<torrentsync::dht::message::FindNode*>(m.get());
    BOOST_REQUIRE(p);

    BOOST_REQUIRE_EQUAL(p->getID(),"abcdefghij0123456789");
    BOOST_REQUIRE_EQUAL(p->getTransactionID(),"aa");
    BOOST_REQUIRE_NO_THROW(
            BOOST_REQUIRE_EQUAL(p->getTarget(),"mnopqrstuvwxyz123456"));
    BOOST_REQUIRE_THROW(p->getNodes(),
        MalformedMessageException);
}

BOOST_AUTO_TEST_SUITE_END()
