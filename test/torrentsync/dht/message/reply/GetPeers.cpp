#include <gtest/gtest.h>

#include <torrentsync/dht/message/reply/GetPeers.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/Node.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>



using namespace torrentsync::dht::message;
using namespace torrentsync;

TEST(GetPeers, reply_nodes) {
  const std::string id1("4747474747474747474747474747474747474747");
  const std::string id2("4848484848484848484848484848484848484848");
  const std::string token("46464646");
  const auto transaction = utils::makeBuffer("aa");

  auto buff = utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes52:"
                                "HHHHHHHHHHHHHHHHHHHHEEGGFDHHHHHHHHHHHHHHHHHHHH"
                                "EEGGFD5:token8:46464646e1:t2:aa1:y1:re");

  dht::NodeData source(utils::parseIDFromHex(id1));
  dht::NodeData target(utils::parseIDFromHex(id2));

  // 0x4747 => 'GG', 0x45454545 => 'EEEE'
  boost::optional<boost::asio::ip::udp::endpoint> endpoint(
      boost::asio::ip::udp::endpoint(boost::asio::ip::address_v4(0x45454747),
                                     0x4644));

  std::shared_ptr<dht::Node> match(new dht::Node(target.write(), endpoint));

  std::list<dht::NodeSPtr> nodes;
  nodes.push_back(match);
  nodes.push_back(match);

  utils::Buffer ret;
  EXPECT_NO_THROW(ret = reply::GetPeers::make(transaction,
                                                     utils::makeBuffer(token),
                                                     source, nodes));

  ASSERT_TRUE(ret == buff);

  const auto m = parseMessage(ret);
  EXPECT_TRUE(getType(m) == Type::Reply);
  EXPECT_TRUE(getTransactionID(m) == transaction);

  std::vector<dht::NodeSPtr> peers;
  auto *r = boost::get<reply::Reply>(&m);
  ASSERT_TRUE(r != nullptr);
  auto *get_peers = boost::get<reply::GetPeers>(r);

  EXPECT_NO_THROW(ASSERT_TRUE(!!get_peers->getNodes());
                         ASSERT_TRUE(!get_peers->getPeers()));
  EXPECT_NO_THROW(peers = *get_peers->getNodes());
  EXPECT_EQ(peers.size(), nodes.size());
  EXPECT_TRUE(get_peers->getType() == Type::Reply);
  EXPECT_TRUE(get_peers->getTransactionID() == transaction);

  ASSERT_TRUE(peers[0]->write() == utils::makeBuffer("HHHHHHHHHHHHHHHHHHHH"));
  EXPECT_EQ(peers[0]->getEndpoint()->address().to_v4().to_ulong(),
                      0x45454747);
  EXPECT_EQ(peers[0]->getEndpoint()->port(), 0x4644);
}

TEST(GetPeers, reply_nodes_failed) {
  const std::string id1("4747474747474747474747474747474747474747");
  const std::string id2("4848484848484848484848484848484848484848");
  const std::string token("46464646");
  const auto transaction = utils::makeBuffer("aa");

  dht::NodeData source(utils::parseIDFromHex(id1));
  dht::NodeData target(utils::parseIDFromHex(id2));

  std::list<dht::NodeSPtr> nodes;

  utils::Buffer ret;
  EXPECT_THROW(ret = reply::GetPeers::make(
                          transaction, utils::makeBuffer(token), source, nodes),
                      dht::message::MessageException);
}

TEST(GetPeers, reply_peers) {
  const std::string id1("4747474747474747474747474747474747474747");
  const std::string token("46464646");
  const auto transaction = utils::makeBuffer("aa");

  auto buff =
      utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:token8:464646466:"
                        "valuesl6:EEGGFD6:EEGGFDee1:t2:aa1:y1:re");

  dht::NodeData source(utils::parseIDFromHex(id1));

  // 0x4747 => 'GG', 0x45454545 => 'EEEE'
  boost::asio::ip::udp::endpoint endpoint(
      boost::asio::ip::address_v4(0x45454747), 0x4644);

  std::list<boost::asio::ip::udp::endpoint> endpoints;
  endpoints.push_back(endpoint);
  endpoints.push_back(endpoint);

  utils::Buffer ret;
  EXPECT_NO_THROW(ret = reply::GetPeers::make(transaction,
                                                     utils::makeBuffer(token),
                                                     source, endpoints));

  ASSERT_TRUE(ret == buff);

  const auto m = parseMessage(ret);
  EXPECT_TRUE(getType(m) == Type::Reply);
  EXPECT_TRUE(getTransactionID(m) == transaction);

  auto *r = boost::get<reply::Reply>(&m);
  ASSERT_TRUE(r != nullptr);
  auto *get_peers = boost::get<reply::GetPeers>(r);
  ASSERT_TRUE(get_peers != nullptr);

  std::list<boost::asio::ip::udp::endpoint> peers;

  EXPECT_TRUE(get_peers->getType() == Type::Reply);
  EXPECT_TRUE(get_peers->getTransactionID() == transaction);
  EXPECT_NO_THROW(ASSERT_TRUE(!get_peers->getNodes());
                         ASSERT_TRUE(!!get_peers->getPeers()));
  EXPECT_NO_THROW(peers = *get_peers->getPeers());

  EXPECT_EQ(peers.size(), endpoints.size());
  ASSERT_TRUE(*peers.begin() == *endpoints.begin());
  ASSERT_TRUE(*(++peers.begin()) == *(++endpoints.begin()));
}

TEST(GetPeers, reply_peers_failed) {
  const std::string id1("4747474747474747474747474747474747474747");
  const std::string token("46464646");
  const auto transaction = utils::makeBuffer("aa");

  dht::NodeData source(utils::parseIDFromHex(id1));

  std::list<boost::asio::ip::udp::endpoint> nodes;

  utils::Buffer ret;
  EXPECT_THROW(ret = reply::GetPeers::make(
                          transaction, utils::makeBuffer(token), source, nodes),
                      dht::message::MessageException);
}


