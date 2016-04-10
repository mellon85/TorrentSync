#include <gtest/gtest.h>

#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>



using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::reply;
using namespace torrentsync;

TEST(Ping, reply) {
  const utils::Buffer transactionID = {static_cast<uint8_t>(rand()),
                                       static_cast<uint8_t>(rand())};
  const auto addr = dht::NodeData::getRandom();
  const utils::Buffer buffer =
      dht::message::reply::Ping::make(transactionID, addr);
  EXPECT_NO_THROW(
      const auto mm = parseMessage(buffer);
      auto *r = boost::get<torrentsync::dht::message::reply::Reply>(&mm);
      ASSERT_TRUE(r != nullptr);
      auto *m = boost::get<torrentsync::dht::message::reply::Ping>(r);
      ASSERT_TRUE(m != nullptr);

      ASSERT_TRUE(m->getType() == Type::Reply);
      ASSERT_TRUE(m->getTransactionID() == transactionID););
}


