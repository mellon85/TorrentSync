#include <boost/test/unit_test.hpp>
#include <sstream>

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/Node.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

#pragma message "Incomplete test suite. still failing."

/*
BOOST_AUTO_TEST_SUITE(torrentsync_dht_Callback);

using namespace torrentsync::dht;

static const torrentsync::utils::Buffer buff("GGGGGGGGHHHHHHHHIIII",20);
static const torrentsync::utils::Buffer transaction("aa");

boost::shared_ptr<torrentsync::dht::message::Message> getMessage()
{
    torrentsync::utils::Buffer b("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");
    std::stringstream s;
    s.write(b.get(),b.size());

    auto m = boost::shared_ptr<torrentsync::dht::message::Message>(
            torrentsync::dht::message::Message::parseMessage(s));

    return m;
}

BOOST_AUTO_TEST_CASE(match_type_fail)
{
    int v = 0;

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            {
            v += t.getType().empty() ? 0 : 1;
            return true;},
        torrentsync::dht::message::Type::Response,
        torrentsync::dht::message::Messages::Ping,
        Callback::filterNode(),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(v,0);
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);

    BOOST_REQUIRE_EQUAL(true,call.verifyConstraints(*getMessage()));
}

BOOST_AUTO_TEST_CASE(match_type_success)
{
    int v = 0;

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            { v += t.getType().empty() ? 0 : 1;
              return true;},
        torrentsync::dht::message::Type::Query,
        torrentsync::dht::message::Messages::Ping,
        Callback::filterNode(),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(v,0);
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);

    BOOST_REQUIRE_EQUAL(true,call.verifyConstraints(*getMessage()));
}

BOOST_AUTO_TEST_CASE(match_messagetype_fail)
{
    int v = 0;

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            {
            v += t.getType().empty() ? 0 : 1;
            return true;},
        torrentsync::dht::message::Type::Query,
        torrentsync::dht::message::Messages::FindNode,
        Callback::filterNode(),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(v,0);
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);

    BOOST_REQUIRE_EQUAL(true,call.verifyConstraints(*getMessage()));
}

BOOST_AUTO_TEST_CASE(match_node_success)
{
    int v = 0;

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            {
            v += t.getType().empty() ? 0 : 1;
            return true;},
        torrentsync::dht::message::Type::Query,
        torrentsync::dht::message::Messages::Ping,
        Callback::filterNode(buff),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(v,0);
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);

    BOOST_REQUIRE_EQUAL(true,call.verifyConstraints(*getMessage()));
}

BOOST_AUTO_TEST_CASE(match_node_failure)
{
    int v = 0;

    torrentsync::utils::Buffer buff2("G000GGG0HHHHHHHHIIII");

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            {
            v += t.getType().empty() ? 0 : 1;
            return true;},
        torrentsync::dht::message::Type::Query,
        torrentsync::dht::message::Messages::Ping,
        Callback::filterNode(buff2),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(false,call.verifyConstraints(*getMessage()));
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);
}

BOOST_AUTO_TEST_CASE(match_transaction_success)
{
    int v = 0;

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            {
            v += t.getType().empty() ? 0 : 1;
            return true;},
        torrentsync::dht::message::Type::Query,
        torrentsync::dht::message::Messages::Ping,
        Callback::filterNode(),
        Callback::filterTransactionID(transaction));

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(false,call.verifyConstraints(*getMessage()));
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);
}

BOOST_AUTO_TEST_CASE(match_transaction_failure)
{
    int v = 0;
    const torrentsync::utils::Buffer transaction2("a2");

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            {
            v += t.getType().empty() ? 0 : 1;
            return true;},
        torrentsync::dht::message::Type::Query,
        torrentsync::dht::message::Messages::Ping,
        Callback::filterNode(),
        Callback::filterTransactionID(transaction2));

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(false,call.verifyConstraints(*getMessage()));
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);
}


BOOST_AUTO_TEST_SUITE_END();

*/
