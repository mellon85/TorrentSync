#include <gtest/gtest.h>
#include <sstream>

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/Node.h>
#include <test/torrentsync/dht/CommonNodeTest.h>



using namespace torrentsync;
namespace msg = torrentsync::dht::message;

using torrentsync::dht::Callback;

static const dht::NodeData buff(utils::makeBuffer("GGGGGGGGHHHHHHHHIIII"));
static const utils::Buffer transaction = utils::makeBuffer("aa");

static msg::AnyMessage getMessage() {
  std::stringstream s;
  s << "d1:ad2:id20:GGGGGGGGHHHHHHHHIIIIe1:q4:ping1:t2:aa1:y1:qe";

  return msg::parseMessage(s);
}

static dht::Node node(utils::makeBuffer("01234567890123456789"));

TEST(Callback, match_node_success) {
  int v = 0;

  Callback call(
      [&v](boost::optional<Callback::payload_type> data,
           const dht::Callback &) -> void {
        ASSERT_TRUE(!!data);
        v += msg::getType(data->message).empty() ? 0 : 1;
      },
      buff, transaction);

  EXPECT_EQ(false, call.isOld());
  EXPECT_EQ(v, 0);
  call.call(getMessage(), node);
  EXPECT_EQ(v, 1);

  EXPECT_EQ(true, call.verifyConstraints(getMessage()));
}

TEST(Callback, match_node_failure) {
  int v = 0;

  utils::Buffer buff2 = utils::makeBuffer("G000GGG0HHHHHHHHIIII");

  Callback call(
      [&v](boost::optional<Callback::payload_type> data,
           const dht::Callback &) -> void {
        ASSERT_TRUE(!!data);
        v += msg::getType(data->message).empty() ? 0 : 1;
      },
      dht::NodeData(buff2), transaction);

  EXPECT_EQ(false, call.isOld());
  EXPECT_EQ(false, call.verifyConstraints(getMessage()));
  call.call(getMessage(), node);
  EXPECT_EQ(v, 1);
}

TEST(Callback, match_transaction_success) {
  int v = 0;

  Callback call(
      [&v](boost::optional<Callback::payload_type> data,
           const dht::Callback &) -> void {
        ASSERT_TRUE(!!data);
        v += msg::getType(data->message).empty() ? 0 : 1;
      },
      buff, transaction);

  EXPECT_EQ(false, call.isOld());
  EXPECT_EQ(true, call.verifyConstraints(getMessage()));
  call.call(getMessage(), node);
  EXPECT_EQ(v, 1);
}

TEST(Callback, match_transaction_failure) {
  int v = 0;
  auto transaction2 = utils::makeBuffer("a2");

  Callback call(
      [&v](boost::optional<Callback::payload_type> data,
           const dht::Callback &) -> void {
        ASSERT_TRUE(!!data);
        v += msg::getType(data->message).empty() ? 0 : 1;
      },
      buff, transaction2);

  EXPECT_EQ(false, call.isOld());
  EXPECT_EQ(false, call.verifyConstraints(getMessage()));
  call.call(getMessage(), node);
  EXPECT_EQ(v, 1);
}


