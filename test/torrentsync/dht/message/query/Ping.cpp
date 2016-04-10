#include <gtest/gtest.h>

#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>



using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::query;
using namespace torrentsync;

TEST(Ping, generation_1) {
  utils::Buffer ret;

  auto transaction = utils::makeBuffer("aa");
  utils::Buffer b = {71, 71, 71, 71, 71, 71, 71, 71, 72, 72,
                     72, 72, 72, 72, 72, 72, 73, 73, 73, 73};
  dht::NodeData data;
  data.read(b.cbegin(), b.cend());

  EXPECT_NO_THROW(ret = Ping::make(transaction, data));
  ASSERT_TRUE(
      utils::makeBuffer(
          "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe") == ret);
}

TEST(Ping, parse) {
  auto b = utils::makeBuffer(
      "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");

  auto m = parseMessage(b);
  ASSERT_TRUE(getType(m) == Type::Query);

  auto *q = boost::get<query::Query>(&m);
  ASSERT_TRUE(q != nullptr);
  auto *p = boost::get<query::Ping>(q);
  ASSERT_TRUE(p != nullptr);

  ASSERT_TRUE(getID(m) == "GGGGGGGGHHHHHHHHIIII");
  ASSERT_TRUE(getTransactionID(m) == "aa");
}

TEST(Ping, parseBinary) {
  auto b = utils::makeBuffer(
      "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");
  b[15] = '\t';
  b[18] = '\0';

  auto m = parseMessage(b);
  ASSERT_TRUE(getType(m) == Type::Query);

  auto *q = boost::get<query::Query>(&m);
  ASSERT_TRUE(q != nullptr);
  auto *p = boost::get<query::Ping>(q);
  ASSERT_TRUE(p != nullptr);

  auto id = utils::makeBuffer("GGGGGGGGHHHHHHHHIIII");
  id[3] = '\t';
  id[6] = '\0';

  ASSERT_TRUE(getID(m) == id);
  ASSERT_TRUE(getTransactionID(m) == "aa");
}

TEST(Ping, parseRandom) {
  for (size_t i = 0; i < TEST_LOOP_COUNT; ++i) {
    //! set address to 0s and then copy something inside
    auto b = utils::makeBuffer(
        "d1:ad2:id20:00000000000000000000e1:q4:ping1:t2:aa1:y1:qe");

    auto addr = dht::NodeData::getRandom();
    auto ab = addr.write();
    std::copy(ab.cbegin(), ab.cend(), b.begin() + 12);

    auto m = parseMessage(b);
    ASSERT_TRUE(getType(m) == Type::Query);

    auto *q = boost::get<query::Query>(&m);
    ASSERT_TRUE(q != nullptr);
    auto *p = boost::get<query::Ping>(q);
    ASSERT_TRUE(p != nullptr);

    ASSERT_TRUE(getID(m) == ab);
    ASSERT_TRUE(getTransactionID(m) == "aa");
  }
}


