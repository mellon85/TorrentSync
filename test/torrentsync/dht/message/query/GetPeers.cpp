#include <sstream>
#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/query/GetPeers.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/Node.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_query_GetPeers);

using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::query;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(generation_and_parse) {
  const auto transaction = utils::makeBuffer("aa");
  const auto info_hash = utils::makeBuffer("abcdefghij0123456789");
  const auto node = utils::makeBuffer("9876543210jihgfedcba");

  auto target = dht::NodeData(node);
  utils::Buffer ret;

  BOOST_REQUIRE_NO_THROW(ret = GetPeers::make(transaction, info_hash, target));
  BOOST_REQUIRE(ret == "d1:ad2:id20:9876543210jihgfedcba9:info_hash20:"
                       "abcdefghij0123456789e1:q9:get_peers1:t2:aa1:y1:qe");
}

BOOST_AUTO_TEST_CASE(parse) {
  auto buff =
      utils::makeBuffer("d1:ad2:id20:9876543210jihgfedcba9:info_hash20:"
                        "abcdefghij0123456789e1:q9:get_peers1:t2:bb1:y1:qe");
  auto mm = parseMessage(buff);

  auto *q = boost::get<query::Query>(&mm);
  BOOST_REQUIRE(q != nullptr);
  auto *m = boost::get<query::GetPeers>(q);
  BOOST_REQUIRE(m != nullptr);
  BOOST_CHECK(getType(mm) == Type::Query);

  BOOST_REQUIRE(m->getID() == "9876543210jihgfedcba");
  BOOST_REQUIRE(m->getTransactionID() == "bb");

  BOOST_REQUIRE(m->getInfoHash() == "abcdefghij0123456789");
}

BOOST_AUTO_TEST_SUITE_END();
