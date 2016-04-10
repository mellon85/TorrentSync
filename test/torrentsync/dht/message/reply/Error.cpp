#include <gtest/gtest.h>

#include <torrentsync/dht/message/reply/Error.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>



using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::reply;
using namespace torrentsync;

TEST(Error, reply) {
  const utils::Buffer transactionID = {'a', 'a'};

  const utils::Buffer buffer =
      dht::message::reply::Error::make(transactionID, ErrorType::protocolError);

  ASSERT_TRUE(buffer == utils::makeBuffer(
                              "d1:eli203e14:Protocol Errore1:t2:aa1:y1:ee"));

  auto m = parseMessage(buffer);
  auto *r = boost::get<Reply>(&m);
  ASSERT_TRUE(r != nullptr);
  auto *ptr = boost::get<Error>(r);

  ASSERT_TRUE(ptr->getType() == dht::message::Type::Error);
  ASSERT_TRUE(ptr->getTransactionID() == transactionID);
  EXPECT_THROW(transactionID == ptr->getID(), std::runtime_error);
}


