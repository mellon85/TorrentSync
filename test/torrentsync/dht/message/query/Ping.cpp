#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/NodeData.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_query_Ping);

using namespace torrentsync::dht::message;
using namespace torrentsync::dht::message::query;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(generation_1) {
  utils::Buffer ret;

  auto transaction = utils::makeBuffer("aa");
  utils::Buffer b = {71, 71, 71, 71, 71, 71, 71, 71, 72, 72,
                     72, 72, 72, 72, 72, 72, 73, 73, 73, 73};
  dht::NodeData data;
  data.read(b.cbegin(), b.cend());

  BOOST_REQUIRE_NO_THROW(ret = Ping::make(transaction, data));
  BOOST_REQUIRE(
      utils::makeBuffer(
          "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe") == ret);
}

BOOST_AUTO_TEST_CASE(parse) {
  auto b = utils::makeBuffer(
      "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");

  auto m = parseMessage(b);
  BOOST_REQUIRE(getType(m) == Type::Query);

  auto *q = boost::get<query::Query>(&m);
  BOOST_REQUIRE(q != nullptr);
  auto *p = boost::get<query::Ping>(q);
  BOOST_REQUIRE(p != nullptr);

  BOOST_REQUIRE(getID(m) == "GGGGGGGGHHHHHHHHIIII");
  BOOST_REQUIRE(getTransactionID(m) == "aa");
}

BOOST_AUTO_TEST_CASE(parseBinary) {
  auto b = utils::makeBuffer(
      "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe");
  b[15] = '\t';
  b[18] = '\0';

  auto m = parseMessage(b);
  BOOST_REQUIRE(getType(m) == Type::Query);

  auto *q = boost::get<query::Query>(&m);
  BOOST_REQUIRE(q != nullptr);
  auto *p = boost::get<query::Ping>(q);
  BOOST_REQUIRE(p != nullptr);

  auto id = utils::makeBuffer("GGGGGGGGHHHHHHHHIIII");
  id[3] = '\t';
  id[6] = '\0';

  BOOST_REQUIRE(getID(m) == id);
  BOOST_REQUIRE(getTransactionID(m) == "aa");
}

BOOST_AUTO_TEST_CASE(parseRandom) {
  for (size_t i = 0; i < TEST_LOOP_COUNT; ++i) {
    //! set address to 0s and then copy something inside
    auto b = utils::makeBuffer(
        "d1:ad2:id20:00000000000000000000e1:q4:ping1:t2:aa1:y1:qe");

    auto addr = dht::NodeData::getRandom();
    auto ab = addr.write();
    std::copy(ab.cbegin(), ab.cend(), b.begin() + 12);

    auto m = parseMessage(b);
    BOOST_REQUIRE(getType(m) == Type::Query);

    auto *q = boost::get<query::Query>(&m);
    BOOST_REQUIRE(q != nullptr);
    auto *p = boost::get<query::Ping>(q);
    BOOST_REQUIRE(p != nullptr);

    BOOST_REQUIRE(getID(m) == ab);
    BOOST_REQUIRE(getTransactionID(m) == "aa");
  }
}

BOOST_AUTO_TEST_SUITE_END()
