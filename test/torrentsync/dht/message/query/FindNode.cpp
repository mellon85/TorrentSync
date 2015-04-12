#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/utils/Yield.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>


BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_query_FindNode);

using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::query;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(generation_1)
{
    utils::Buffer ret;

    auto transaction = utils::makeBuffer("aa");
    utils::Buffer b = {'a','b','c','d','e','f','g','h','i','j','0','1','2','3','4','5','6','7','8','9'};
    utils::Buffer tb = {'m','n','o','p','q','r','s','t','u','v','w','x','y','z','1','2','3','4','5','6'};

    dht::NodeData data, target;
    data.read(b.cbegin(),b.cend());
    target.read(tb.cbegin(),tb.cend());

    BOOST_REQUIRE_NO_THROW(
        ret = FindNode::make(transaction,data,target)); 
    BOOST_REQUIRE(
        ret == "d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe");
}

BOOST_AUTO_TEST_CASE(parse)
{
    auto b = utils::makeBuffer("d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe");

    auto m = std::dynamic_pointer_cast<Query>(
            std::shared_ptr<Message>(Message::parseMessage(b)));
    BOOST_REQUIRE(!!m);
    BOOST_REQUIRE(m->getType() == Type::Query);
    BOOST_REQUIRE(m->getMessageType() == Messages::FindNode);

    auto p = std::dynamic_pointer_cast<FindNode>(m);
    BOOST_REQUIRE(p.get());

    BOOST_REQUIRE(p->getID() == "abcdefghij0123456789");
    BOOST_REQUIRE(p->getTransactionID() == "aa");
    BOOST_REQUIRE_NO_THROW(
            BOOST_REQUIRE(p->getTarget() == "mnopqrstuvwxyz123456"));
}

BOOST_AUTO_TEST_SUITE_END();
