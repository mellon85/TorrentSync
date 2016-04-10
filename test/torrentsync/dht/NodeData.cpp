
#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

#include <stdexcept>
#include <torrentsync/dht/NodeData.h>
#include <test/torrentsync/dht/CommonNodeTest.h>



using namespace torrentsync::dht;
using namespace torrentsync;

TEST(NodeData, rand_data) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    const std::string data = generateRandomNode();
    EXPECT_NO_THROW(
        const utils::Buffer buff = utils::parseIDFromHex(data);
        NodeData node(buff); EXPECT_EQ(data, node.string()););
  }
}

TEST(NodeData, too_short) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::string s;
    int length = rand() % 39;
    length += length % 2;
    s.assign(length, '1');
    EXPECT_THROW(NodeData(utils::parseIDFromHex(s)),
                        std::invalid_argument);
  }
}

TEST(NodeData, too_long) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::string s;
    int length = rand() % TEST_LOOP_COUNT + 40 + 1;
    length += length % 2;
    s.assign(length, '1');
    EXPECT_THROW(NodeData(utils::parseIDFromHex(s)),
                        std::invalid_argument);
  }
}

TEST(NodeData, comparing_fixed1) {
  NodeData a1(
      utils::parseIDFromHex("0F00000000000000000000000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("FF00000000000000000000000000000000000000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  EXPECT_EQ(a2 > a1, true);
  EXPECT_EQ(a2 >= a1, true);
  EXPECT_EQ(a2 < a1, false);
  EXPECT_EQ(a2 <= a1, false);
  EXPECT_EQ(a2 == a1, false);

  EXPECT_EQ(a3 > a2, true);
  EXPECT_EQ(a3 >= a2, true);
  EXPECT_EQ(a3 > a1, true);
  EXPECT_EQ(a3 >= a1, true);
  EXPECT_EQ(a3 < a2, false);
  EXPECT_EQ(a3 <= a2, false);
  EXPECT_EQ(a3 == a2, false);
  EXPECT_EQ(a3 < a1, false);
  EXPECT_EQ(a3 <= a1, false);
  EXPECT_EQ(a3 == a1, false);
}

TEST(NodeData, comparing_fixed2) {
  NodeData a1(
      utils::parseIDFromHex("0000000000000000000000000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("0000000000000000000F00000000000000000000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  EXPECT_EQ(a2 > a1, true);
  EXPECT_EQ(a2 >= a1, true);
  EXPECT_EQ(a2 < a1, false);
  EXPECT_EQ(a2 <= a1, false);
  EXPECT_EQ(a2 == a1, false);

  EXPECT_EQ(a3 > a2, true);
  EXPECT_EQ(a3 >= a2, true);
  EXPECT_EQ(a3 > a1, true);
  EXPECT_EQ(a3 >= a1, true);
  EXPECT_EQ(a3 < a2, false);
  EXPECT_EQ(a3 <= a2, false);
  EXPECT_EQ(a3 == a2, false);
  EXPECT_EQ(a3 < a1, false);
  EXPECT_EQ(a3 <= a1, false);
  EXPECT_EQ(a3 == a1, false);
}

TEST(NodeData, comparing_fixed3) {
  NodeData a1(
      utils::parseIDFromHex("000F000000000000000000000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("000F000000000000000F00000000000000000000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  EXPECT_EQ(a2 > a1, true);
  EXPECT_EQ(a2 >= a1, true);
  EXPECT_EQ(a2 < a1, false);
  EXPECT_EQ(a2 <= a1, false);
  EXPECT_EQ(a2 == a1, false);

  EXPECT_EQ(a3 > a2, true);
  EXPECT_EQ(a3 >= a2, true);
  EXPECT_EQ(a3 > a1, true);
  EXPECT_EQ(a3 >= a1, true);
  EXPECT_EQ(a3 < a2, false);
  EXPECT_EQ(a3 <= a2, false);
  EXPECT_EQ(a3 == a2, false);
  EXPECT_EQ(a3 < a1, false);
  EXPECT_EQ(a3 <= a1, false);
  EXPECT_EQ(a3 == a1, false);
}

TEST(NodeData, comparing_fixed4) {
  NodeData a1(
      utils::parseIDFromHex("000F000000000000000F00000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("000F000000000000000F00000000000000F00000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  EXPECT_EQ(a2 > a1, true);
  EXPECT_EQ(a2 >= a1, true);
  EXPECT_EQ(a2 < a1, false);
  EXPECT_EQ(a2 <= a1, false);
  EXPECT_EQ(a2 == a1, false);

  EXPECT_EQ(a3 > a2, true);
  EXPECT_EQ(a3 >= a2, true);
  EXPECT_EQ(a3 > a1, true);
  EXPECT_EQ(a3 >= a1, true);
  EXPECT_EQ(a3 < a2, false);
  EXPECT_EQ(a3 <= a2, false);
  EXPECT_EQ(a3 == a2, false);
  EXPECT_EQ(a3 < a1, false);
  EXPECT_EQ(a3 <= a1, false);
  EXPECT_EQ(a3 == a1, false);
}

TEST(NodeData, splitInHalf_ok) {
  NodeData low = NodeData::minValue;
  NodeData high = NodeData::maxValue;

  MaybeBounds bounds = NodeData::splitInHalf(low, high);

  ASSERT_TRUE(!!bounds);
  EXPECT_LT(low, bounds->first);
  EXPECT_LT(bounds->first, bounds->second);
  EXPECT_LT(bounds->second, high);

  EXPECT_EQ(bounds->first.string(),
                      "7fffffffffffffffffffffffffffffffffffffff");
  EXPECT_EQ(bounds->second.string(),
                      "8000000000000000000000000000000000000000");

  // sub bounds low-bounds.
  MaybeBounds bounds_low = NodeData::splitInHalf(low, bounds->first);

  ASSERT_TRUE(!!bounds_low);
  EXPECT_LT(low, bounds_low->first);
  EXPECT_LT(bounds_low->first, bounds_low->second);
  EXPECT_LT(bounds_low->second, bounds->first);

  EXPECT_EQ(bounds_low->first.string(),
                      "3fffffffffffffffffffffffffffffffffffffff");
  EXPECT_EQ(bounds_low->second.string(),
                      "4000000000000000000000000000000000000000");

  // sub bounds high-bounds.
  MaybeBounds bounds_high = NodeData::splitInHalf(bounds->second, high);

  ASSERT_TRUE(!!bounds_high);
  EXPECT_LT(bounds->second, bounds_high->first);
  EXPECT_LT(bounds_high->first, bounds_high->second);
  EXPECT_LT(bounds_high->second, high);

  EXPECT_EQ(bounds_high->first.string(),
                      "bfffffffffffffffffffffffffffffffffffffff");
  EXPECT_EQ(bounds_high->second.string(),
                      "c000000000000000000000000000000000000000");
}

TEST(NodeData, bytestring_parsing) {
  utils::Buffer s = {'1', '2', '3', '4', '5', '6', '7', '8', 'a', 'b',
                     'c', 'd', 'e', 'f', 'g', 'h', 'A', 'B', 'C', 'D'};
  NodeData d;
  d.read(s.begin(), s.cend());
  utils::Buffer b = d.write();
  ASSERT_TRUE(b == s);
}

TEST(NodeData, bytestring_parsing_special) {
  torrentsync::utils::Buffer s = {'1', '2', '3',  '\n', '5', '6', '7',
                                  '8', 'a', '\0', 'c',  'd', 'e', 'f',
                                  'g', 'h', '\0', 'B',  'C', 'D'};
  NodeData d;
  d.read(s.begin(), s.cend());
  torrentsync::utils::Buffer b = d.write();
  ASSERT_TRUE(b == s);
}


