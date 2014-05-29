#include <boost/test/unit_test.hpp>
#include <sstream>

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/Peer.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

using boost::asio::ip::udp;

BOOST_AUTO_TEST_SUITE(torrentsync_dht_Callback);

using namespace torrentsync::dht;


boost::shared_ptr<torrentsync::dht::message::Message> getMessage()
{
    torrentsync::utils::Buffer b("d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");
    std::stringstream s;
    s.write(b.get(),b.size());

    return torrentsync::dht::message::Message::parseMessage(s);
}

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    int v = 0;

    Callback call(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            { ++v; return true;},
        "r",
        Callback::filterPeer(),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
    BOOST_REQUIRE_EQUAL(v,0);
    call.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,1);

    BOOST_REQUIRE_EQUAL(false,call.verifyConstraints(*getMessage()));

    Callback call2(
        [&v]( const torrentsync::dht::message::Message& t) -> bool
            { v = 1; return true;},
        "q",
        Callback::filterPeer(),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(true,call.verifyConstraints(*getMessage()));
    call2.call(*getMessage());
    BOOST_REQUIRE_EQUAL(v,2);
}

/*

BOOST_AUTO_TEST_CASE(match)
{
    const std::string type = "q";

    torrentsync::dht::Node node(
            generateRandomNode());

    int v = 0;

    const Callback::filterPeer peer(node,udp::endpoint());

    Callback call(
        [&v]( const torrentsync::dht::Message& m) { v = 1; },
        type,
        node,
        Callback::filterTransactionID());

}
*/


BOOST_AUTO_TEST_SUITE_END();

