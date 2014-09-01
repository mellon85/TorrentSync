#include <boost/test/unit_test.hpp>
#include <sstream>

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/Node.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_Callback);

using namespace torrentsync;
namespace msg = torrentsync::dht::message;

using torrentsync::dht::Callback;

static const dht::NodeData buff(utils::makeBuffer("GGGGGGGGHHHHHHHHIIII"));
static const utils::Buffer transaction = utils::makeBuffer("aa");

static msg::Message getMessage()
{
    std::stringstream s;
    s << "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe";

    return *(msg::Message::parseMessage(s));
}

static dht::Node node(utils::makeBuffer("01234567890123456789"));

BOOST_AUTO_TEST_CASE(match_node_success)
{
    int v = 0;

    Callback call(
        [&v]( boost::optional<Callback::payload_type> data,
              const dht::Callback& c) -> void {
            BOOST_REQUIRE(!!data);
            v += data->message.getType().empty() ? 0 : 1; },
        buff,
        transaction);

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(v,0);
    call.call(getMessage(),node);
    BOOST_REQUIRE_EQUAL(v,1);

    BOOST_REQUIRE_EQUAL(true,call.verifyConstraints(getMessage()));
}

BOOST_AUTO_TEST_CASE(match_node_failure)
{
    int v = 0;

    utils::Buffer buff2 = utils::makeBuffer("G000GGG0HHHHHHHHIIII");

    Callback call(
        [&v]( boost::optional<Callback::payload_type> data,
              const dht::Callback& c) -> void {
            BOOST_REQUIRE(!!data);
            v += data->message.getType().empty() ? 0 : 1; },
        dht::NodeData(buff2),
        transaction);
        

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(false,call.verifyConstraints(getMessage()));
    call.call(getMessage(),node);
    BOOST_REQUIRE_EQUAL(v,1);
}

BOOST_AUTO_TEST_CASE(match_transaction_success)
{
    int v = 0;

    Callback call(
        [&v]( boost::optional<Callback::payload_type> data,
              const dht::Callback& c) -> void {
            BOOST_REQUIRE(!!data);
            v += data->message.getType().empty() ? 0 : 1; },
        buff,
        transaction);

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(true,call.verifyConstraints(getMessage()));
    call.call(getMessage(),node);
    BOOST_REQUIRE_EQUAL(v,1);
}

BOOST_AUTO_TEST_CASE(match_transaction_failure)
{
    int v = 0;
    auto transaction2 = utils::makeBuffer("a2");

    Callback call(
        [&v]( boost::optional<Callback::payload_type> data,
              const dht::Callback& c) -> void {
            BOOST_REQUIRE(!!data);
            v += data->message.getType().empty() ? 0 : 1; },
        buff,
        transaction2);

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(false,call.verifyConstraints(getMessage()));
    call.call(getMessage(),node);
    BOOST_REQUIRE_EQUAL(v,1);
}

BOOST_AUTO_TEST_SUITE_END();
