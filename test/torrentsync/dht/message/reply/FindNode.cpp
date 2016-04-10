#include <gtest/gtest.h>

#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/Node.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>



using namespace torrentsync::dht::message;
using namespace torrentsync;

TEST(FindNode, reply_perfectMatch) {
  const std::string id1("4747474747474747474747474747474747474747");
  const std::string id2("4848484848484848484848484848484848484848");
  const auto transaction = utils::makeBuffer("aa");

  const auto buff =
      utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes26:"
                        "HHHHHHHHHHHHHHHHHHHHEEFHGFe1:t2:aa1:y1:re");

  dht::NodeData source(utils::parseIDFromHex(id1));
  dht::NodeData target(utils::parseIDFromHex(id2));

  // 0x4747 => 'GG', 0x45454647 => 'EEDF'
  const auto endpoint(boost::asio::ip::udp::endpoint(
      boost::asio::ip::address_v4(0x45454648), 0x4746));

  std::shared_ptr<dht::Node> match(new dht::Node(target.write(), endpoint));

  std::list<dht::NodeSPtr> nodes;
  nodes.push_back(match);

  utils::Buffer ret;
  EXPECT_NO_THROW(ret =
                             reply::FindNode::make(transaction, source, nodes));

  ASSERT_TRUE(ret == buff);

  const auto m = parseMessage(ret);
  EXPECT_TRUE(getType(m) == Type::Reply);
  EXPECT_TRUE(getTransactionID(m) == transaction);

  auto *r = boost::get<reply::Reply>(&m);
  ASSERT_TRUE(r != nullptr);
  auto *find_node = boost::get<reply::FindNode>(r);
  std::vector<dht::NodeSPtr> peers;
  EXPECT_NO_THROW(peers = find_node->getNodes());
  EXPECT_EQ(peers.size(), nodes.size());
  EXPECT_TRUE(find_node->getType() == Type::Reply);
  EXPECT_TRUE(find_node->getTransactionID() == transaction);

  ASSERT_TRUE(peers[0]->write() == utils::makeBuffer("HHHHHHHHHHHHHHHHHHHH"));
  EXPECT_EQ(peers[0]->getEndpoint()->address().to_v4().to_ulong(),
                      endpoint.address().to_v4().to_ulong());
  EXPECT_EQ(peers[0]->getEndpoint()->port(), endpoint.port());
}

TEST(FindNode, reply_multiple) {
  const std::string id1("4747474747474747474747474747474747474747");
  const std::string id2("4848484848484848484848484848484848484848");
  const std::string id3("4141414141414141414141414141414141414141");
  const std::string id4("4242424242424242424242424242424242424242");
  const auto transaction = utils::makeBuffer("aa");

  auto buff =
      utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes78:"
                        "HHHHHHHHHHHHHHHHHHHHEEGGFDAAAAAAAAAAAAAAAAAAAAEEGGFDBB"
                        "BBBBBBBBBBBBBBBBBBEEGGFDe1:t2:aa1:y1:re");

  dht::NodeData source(utils::parseIDFromHex(id1));
  dht::NodeData target1(utils::parseIDFromHex(id2));
  dht::NodeData target2(utils::parseIDFromHex(id3));
  dht::NodeData target3(utils::parseIDFromHex(id4));

  boost::optional<boost::asio::ip::udp::endpoint> endpoint(
      boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4(0x45454747),
                                     0x4644));

  std::list<dht::NodeSPtr> nodes;
  EXPECT_NO_THROW(
      nodes.push_back(dht::NodeSPtr(new dht::Node(target1.write(), endpoint)));
      nodes.push_back(dht::NodeSPtr(new dht::Node(target2.write(), endpoint)));
      nodes.push_back(
          dht::NodeSPtr(new dht::Node(target3.write(), endpoint))););

  utils::Buffer ret;
  EXPECT_NO_THROW(ret =
                             reply::FindNode::make(transaction, source, nodes));

  ASSERT_TRUE(ret == buff);
  const auto m = parseMessage(ret);

  EXPECT_TRUE(getType(m) == Type::Reply);
  EXPECT_TRUE(getTransactionID(m) == transaction);

  auto *r = boost::get<reply::Reply>(&m);
  ASSERT_TRUE(r != nullptr);
  auto *find_node = boost::get<reply::FindNode>(r);

  std::vector<dht::NodeSPtr> peers;
  EXPECT_NO_THROW(peers = find_node->getNodes());
  EXPECT_EQ(peers.size(), nodes.size());
  EXPECT_TRUE(find_node->getType() == Type::Reply);
  EXPECT_TRUE(find_node->getTransactionID() == transaction);

  EXPECT_TRUE(peers[0]->write() == utils::makeBuffer("HHHHHHHHHHHHHHHHHHHH"));
  EXPECT_EQ(peers[0]->getEndpoint()->address().to_v4().to_ulong(),
                    0x45454747);
  EXPECT_EQ(peers[0]->getEndpoint()->port(), 0x4644);

  EXPECT_TRUE(peers[1]->write() == utils::makeBuffer("AAAAAAAAAAAAAAAAAAAA"));
  EXPECT_EQ(peers[1]->getEndpoint()->address().to_v4().to_ulong(),
                    0x45454747);
  EXPECT_EQ(peers[1]->getEndpoint()->port(), 0x4644);

  EXPECT_TRUE(peers[2]->write() == utils::makeBuffer("BBBBBBBBBBBBBBBBBBBB"));
  EXPECT_EQ(peers[2]->getEndpoint()->address().to_v4().to_ulong(),
                    0x45454747);
  EXPECT_EQ(peers[2]->getEndpoint()->port(), 0x4644);
}


