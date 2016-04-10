#include <sstream>
#include <gtest/gtest.h>

#include <torrentsync/dht/message/query/GetPeers.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/Node.h>

#include <test/torrentsync/dht/CommonNodeTest.h>



using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::query;
using namespace torrentsync;

TEST(GetPeers, generation_and_parse) {
  const auto transaction = utils::makeBuffer("aa");
  const auto info_hash = utils::makeBuffer("abcdefghij0123456789");
  const auto node = utils::makeBuffer("9876543210jihgfedcba");

  auto target = dht::NodeData(node);
  utils::Buffer ret;

  EXPECT_NO_THROW(ret = GetPeers::make(transaction, info_hash, target));
  ASSERT_TRUE(ret == "d1:ad2:id20:9876543210jihgfedcba9:info_hash20:"
                       "abcdefghij0123456789e1:q9:get_peers1:t2:aa1:y1:qe");
}

TEST(GetPeers, parse) {
  auto buff =
      utils::makeBuffer("d1:ad2:id20:9876543210jihgfedcba9:info_hash20:"
                        "abcdefghij0123456789e1:q9:get_peers1:t2:bb1:y1:qe");
  auto mm = parseMessage(buff);

  auto *q = boost::get<query::Query>(&mm);
  ASSERT_TRUE(q != nullptr);
  auto *m = boost::get<query::GetPeers>(q);
  ASSERT_TRUE(m != nullptr);
  EXPECT_TRUE(getType(mm) == Type::Query);

  ASSERT_TRUE(m->getID() == "9876543210jihgfedcba");
  ASSERT_TRUE(m->getTransactionID() == "bb");

  ASSERT_TRUE(m->getInfoHash() == "abcdefghij0123456789");
}


