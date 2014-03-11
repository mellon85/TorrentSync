#include <boost/test/unit_test.hpp>
#include <torrentsync/dht/Peer.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/NodeData.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_Peer);

using namespace torrentsync::dht;

BOOST_AUTO_TEST_CASE(constructor)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        BOOST_REQUIRE_NO_THROW(
            udp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"),0);
            Peer n( Node(generateRandomNode()), endpoint);
            BOOST_REQUIRE_EQUAL(endpoint,n.getEndpoint());
         );
    }
}

BOOST_AUTO_TEST_CASE(parsePeerNode)
{
    torrentsync::utils::Buffer buff(26);
    buff.get()[20] = 1;
    buff.get()[21] = 2;
    buff.get()[22] = 3;
    buff.get()[23] = 4;
    buff.get()[24] = 5;
    buff.get()[25] = 6;

    Peer node;
    node.read(
        buff.begin(), buff.end());

    BOOST_CHECK_EQUAL(node.getEndpoint().port(),0x0506);
    BOOST_REQUIRE_EQUAL(node.getEndpoint().address(),
        boost::asio::ip::address_v4(0x01020304));
}

BOOST_AUTO_TEST_SUITE_END();
