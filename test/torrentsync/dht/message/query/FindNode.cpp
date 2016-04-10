#include <gtest/gtest.h>

#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/Node.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>



using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::query;
using namespace torrentsync;

TEST(FindNode, generation_1) {
  utils::Buffer ret;

  auto transaction = utils::makeBuffer("aa");
  utils::Buffer b = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
                     '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
  utils::Buffer tb = {'m', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                      'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6'};

  dht::NodeData data, target;
  data.read(b.cbegin(), b.cend());
  target.read(tb.cbegin(), tb.cend());

  EXPECT_NO_THROW(ret = FindNode::make(transaction, data, target));
  ASSERT_TRUE(ret == "d1:ad2:id20:abcdefghij01234567896:target20:"
                       "mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe");
}

TEST(FindNode, parse) {
  auto b =
      utils::makeBuffer("d1:ad2:id20:abcdefghij01234567896:target20:"
                        "mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe");

  auto m = parseMessage(b);
  ASSERT_TRUE(getType(m) == Type::Query);
  auto *q = boost::get<Query>(&m);
  auto *p = boost::get<FindNode>(q);

  ASSERT_TRUE(p->getID() == "abcdefghij0123456789");
  ASSERT_TRUE(p->getTransactionID() == "aa");
  EXPECT_NO_THROW(
      ASSERT_TRUE(p->getTarget() == "mnopqrstuvwxyz123456"));
}


