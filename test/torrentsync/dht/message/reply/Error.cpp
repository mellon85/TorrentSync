#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/reply/Error.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>


BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_reply_Error);

using namespace torrentsync::dht::message;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(reply)
{
    const utils::Buffer transactionID = {'a','a'};

    const utils::Buffer buffer = dht::message::reply::Error::make(
            transactionID,ErrorType::protocolError);

    BOOST_REQUIRE(buffer == utils::makeBuffer("d1:eli203e14:Protocol Errore1:t2:aa1:y1:ee"));

    std::shared_ptr<
        dht::message::reply::Error> ptr;

    // parse it back
    BOOST_CHECK_NO_THROW(
        ptr = std::dynamic_pointer_cast<
            dht::message::reply::Error>(dht::message::Message::parseMessage(buffer)););
    BOOST_REQUIRE(!!ptr);

    BOOST_REQUIRE(ptr->getType() == dht::message::Type::Error);
    BOOST_REQUIRE(ptr->getTransactionID() == transactionID);
    BOOST_REQUIRE_THROW(ptr->getID() == transactionID,std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

