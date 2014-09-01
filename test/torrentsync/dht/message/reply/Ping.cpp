#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/reply/Ping.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>


BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_reply_Ping);

using namespace torrentsync::dht::message;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(reply)
{
    const utils::Buffer transactionID = {static_cast<uint8_t>(rand()),
                                         static_cast<uint8_t>(rand())};
    const auto addr = dht::NodeData::getRandom();
    const utils::Buffer buffer = dht::message::reply::Ping::make(transactionID,addr);
    BOOST_REQUIRE_NO_THROW(
    const auto m = dht::message::Message::parseMessage(buffer);
    
    BOOST_REQUIRE(m.get());
    BOOST_REQUIRE(m->getType() == Type::Reply);
    BOOST_REQUIRE(m->getTransactionID() == transactionID););
}

BOOST_AUTO_TEST_SUITE_END()
