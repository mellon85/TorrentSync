#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>
#include <streambuf>

#include <torrentsync/dht/message/BEncodeDecoder.h>



using namespace torrentsync::dht::message;
using namespace torrentsync;
using torrentsync::utils::DataMap;

#define TEST_FIELD(key, value)                                           \
  {                                                                      \
    EXPECT_NE(map.find(utils::makeBuffer(key)), map.end());              \
    EXPECT_EQ(map.find(utils::makeBuffer(key))->second,                  \
                  utils::makeBuffer(value));                             \
  }

TEST(BEncodeDecoder, constructor_destructor) { BEncodeDecoder dec; }

TEST(BEncodeDecoder, parse_emptyDictionary) {
  std::istringstream str("de");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 0);
}

TEST(BEncodeDecoder, parse_oneElementDictionary) {
  std::istringstream str("d1:a1:be");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 1);

  auto t = map.find(utils::makeBuffer("a"))->second;
  auto b = utils::makeBuffer("b");
  TEST_FIELD("a", "b");
}

TEST(BEncodeDecoder, parse_emptyList) {
  std::istringstream str("le");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 0);
}

TEST(BEncodeDecoder, parse_dictionary) {
  std::istringstream str("d1:a2:bb2:yy4:plple");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 2);
  TEST_FIELD("a", "bb");
  TEST_FIELD("yy", "plpl");
}

TEST(BEncodeDecoder, parse_list) {
  std::istringstream str("l1:a2:bb2:yy4:plpli1ee");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 5);
  TEST_FIELD("0", "a");
  TEST_FIELD("1", "bb");
  TEST_FIELD("2", "yy");
  TEST_FIELD("3", "plpl");
  TEST_FIELD("4", 1);
}

TEST(BEncodeDecoder, parse_dictionaryWithList) {
  std::istringstream str("d1:a2:bb2:yy4:plpl1:ql1:a1:b1:cee");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 5);

  TEST_FIELD("a", "bb");
  TEST_FIELD("yy", "plpl");
  TEST_FIELD("q/0", "a");
  TEST_FIELD("q/1", "b");
  TEST_FIELD("q/2", "c");
}

TEST(BEncodeDecoder, parse_dictionaryWithDictionary) {
  std::istringstream str("d1:a2:bb2:yy4:plpl1:qd1:a1:b1:c4:ababee");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 4);

  TEST_FIELD("a", "bb");
  TEST_FIELD("yy", "plpl");
  TEST_FIELD("q/a", "b");
  TEST_FIELD("q/c", "abab");
}

TEST(BEncodeDecoder, parse_dictionaryWithDictionary_inTheMiddle) {
  std::istringstream str("d1:a2:bb2:yy4:plpl1:qd1:a1:b1:c4:ababe1:r1:ce");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 5);

  TEST_FIELD("a", "bb");
  TEST_FIELD("yy", "plpl");
  TEST_FIELD("q/a", "b");
  TEST_FIELD("q/c", "abab");
  TEST_FIELD("r", "c");
}

TEST(BEncodeDecoder, parse_listWithList) {
  std::istringstream str("l1:a2:aa1:ql1:b2:ceee");

  BEncodeDecoder decoder;

  EXPECT_NO_THROW(decoder.parseMessage(str));
  const DataMap &map = decoder.getData();
  EXPECT_EQ(map.size(), 5);

  TEST_FIELD("0", "a");
  TEST_FIELD("1", "aa");
  TEST_FIELD("2", "q");
  TEST_FIELD("3/0", "b");
  TEST_FIELD("3/1", "ce");
}
TEST(BEncodeDecoder, parse_error) {
  std::istringstream str("aaaaa");

  BEncodeDecoder decoder;

  EXPECT_THROW(decoder.parseMessage(str), BEncodeException);
}

TEST(BEncodeDecoder, parse_error2) {
  std::istringstream str("d1:a2:be");

  BEncodeDecoder decoder;

  EXPECT_THROW(decoder.parseMessage(str), BEncodeException);
}

TEST(BEncodeDecoder, parse_error3) {
  std::istringstream str("d1:ab:be");

  BEncodeDecoder decoder;

  EXPECT_THROW(decoder.parseMessage(str), BEncodeException);
}


