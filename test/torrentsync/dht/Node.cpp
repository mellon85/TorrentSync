
#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>

#include <cstdlib>
#include <sstream>
#include <memory>

#include <torrentsync/dht/Node.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

namespace {
class NodeF : public torrentsync::dht::Node, public ::testing::Test {
public:
  NodeF() {}
};
};

using namespace torrentsync;

TEST_F(NodeF, distance_0) {
  const std::string data = "ffffffffffffffff0000000000000001aaaaaaaa";
  auto dataBuff = utils::parseIDFromHex(data);

  std::unique_ptr<Node> addr1;
  EXPECT_NO_THROW(addr1.reset(new Node(dataBuff)));
  std::unique_ptr<Node> addr2;
  EXPECT_NO_THROW(addr2.reset(new Node(dataBuff)));

  EXPECT_EQ(data, addr1->string());
  ASSERT_TRUE(boost::iequals(data, addr2->string()));

  const dht::Distance dist = *addr1 ^ *addr2;
  EXPECT_EQ(dist.string(),
                      "0000000000000000000000000000000000000000");
}

TEST_F(NodeF, distance_some_static) {
  const std::string data1 = "ffffffffffffffff0000000000000001aaaaaaaa";
  auto dataBuff1 = utils::parseIDFromHex(data1);
  const std::string data2 = "ffffffffffffffff0001100000000001aaaaaaaa";
  auto dataBuff2 = utils::parseIDFromHex(data2);

  std::unique_ptr<Node> addr1;
  EXPECT_NO_THROW(addr1.reset(new Node(dataBuff1)));
  std::unique_ptr<Node> addr2;
  EXPECT_NO_THROW(addr2.reset(new Node(dataBuff2)));

  ASSERT_TRUE(boost::iequals(data1, addr1->string()));
  ASSERT_TRUE(boost::iequals(data2, addr2->string()));

  const dht::Distance dist = *addr1 ^ *addr2;
  EXPECT_EQ(dist.string(),
                      "0000000000000000000110000000000000000000");
  ASSERT_TRUE(boost::iequals(data1, addr1->string()));
  ASSERT_TRUE(boost::iequals(data2, addr2->string()));
}

TEST_F(NodeF, distance_random) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::string data1 = generateRandomNode();
    const std::string data2 = data1;

    for (unsigned int numOfDiff = rand() % 40 + 1; numOfDiff > 0; --numOfDiff) {
      const int n = rand() % 16;
      data1[rand() % data1.length()] =
          static_cast<char>(n < 10 ? n + 48 : (n % 6) + 97);
    }

    auto dataBuff1 = utils::parseIDFromHex(data1);
    auto dataBuff2 = utils::parseIDFromHex(data2);

    std::unique_ptr<Node> addr1;
    EXPECT_NO_THROW(addr1.reset(new Node(dataBuff1)));
    std::unique_ptr<Node> addr2;
    EXPECT_NO_THROW(addr2.reset(new Node(dataBuff2)));

    ASSERT_TRUE(boost::iequals(data1, addr1->string()));
    ASSERT_TRUE(boost::iequals(data2, addr2->string()));

    const dht::Distance dist = *addr1 ^ *addr2;
    ASSERT_TRUE(boost::iequals(data1, addr1->string()));
    ASSERT_TRUE(boost::iequals(data2, addr2->string()));

    std::unique_ptr<Node> addr3;
    EXPECT_NO_THROW(
        addr3.reset(new Node(utils::parseIDFromHex(dist.string()))));
    const dht::Distance dist1 = *addr3 ^ *addr2;
    const dht::Distance dist2 = *addr3 ^ *addr1;

    ASSERT_TRUE(boost::iequals(data1, addr1->string()));
    ASSERT_TRUE(boost::iequals(data2, addr2->string()));

    EXPECT_EQ(dist1.string(), addr1->string());
    EXPECT_EQ(dist2.string(), addr2->string());
  }
}

TEST_F(NodeF, is_Good_Bad_Questionale) {
  setGood();
  EXPECT_EQ(true, isGood());
  EXPECT_EQ(isBad(), false);
  EXPECT_EQ(isQuestionable(), false);

  _last_time_good = _last_time_good - good_interval;
  EXPECT_EQ(_last_unanswered_queries, 0);

  EXPECT_EQ(isGood(), false);
  EXPECT_EQ(isQuestionable(), true);
  EXPECT_EQ(isBad(), false);

  _last_unanswered_queries = allowed_unanswered_queries + 1;

  EXPECT_EQ(isGood(), false);
  EXPECT_EQ(isQuestionable(), false);
  EXPECT_EQ(isBad(), true);
}

TEST_F(NodeF, serialization) {
  const utils::Buffer buff = utils::parseIDFromHex(generateRandomNode());
  dht::NodeData nodedata(buff);

  boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::udp::endpoint(
      boost::asio::ip::address_v4(0x44454647), 0x4445));

  dht::Node node1(nodedata.write(), endpoint);
  auto data = node1.getPackedNode();

  dht::Node node2(data.begin(), data.end());

  EXPECT_EQ(node2.getEndpoint()->port(), 0x4445);
  EXPECT_EQ(node2.getEndpoint()->address().to_v4().to_ulong(),
                    0x44454647);
  EXPECT_TRUE(buff == node1.write());
  EXPECT_TRUE(buff == node2.write());
  EXPECT_TRUE(data == node2.getPackedNode());
}


