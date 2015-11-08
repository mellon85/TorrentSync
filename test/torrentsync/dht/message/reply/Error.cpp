#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/reply/Error.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_reply_Error);

using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::reply;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(reply) {
  const utils::Buffer transactionID = {'a', 'a'};

  const utils::Buffer buffer =
      dht::message::reply::Error::make(transactionID, ErrorType::protocolError);

  BOOST_REQUIRE(buffer == utils::makeBuffer(
                              "d1:eli203e14:Protocol Errore1:t2:aa1:y1:ee"));

  auto m = parseMessage(buffer);
  auto *r = boost::get<Reply>(&m);
  BOOST_REQUIRE(r != nullptr);
  auto *ptr = boost::get<Error>(r);

  BOOST_REQUIRE(ptr->getType() == dht::message::Type::Error);
  BOOST_REQUIRE(ptr->getTransactionID() == transactionID);
  BOOST_REQUIRE_THROW(transactionID == ptr->getID(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
