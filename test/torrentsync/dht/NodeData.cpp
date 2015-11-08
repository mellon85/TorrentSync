
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

#include <stdexcept>
#include <torrentsync/dht/NodeData.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_NodeData);

using namespace torrentsync::dht;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(rand_data) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    const std::string data = generateRandomNode();
    BOOST_REQUIRE_NO_THROW(
        const utils::Buffer buff = utils::parseIDFromHex(data);
        NodeData node(buff); BOOST_REQUIRE_EQUAL(data, node.string()););
  }
}

BOOST_AUTO_TEST_CASE(too_short) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::string s;
    int length = rand() % 39;
    length += length % 2;
    s.assign(length, '1');
    BOOST_REQUIRE_THROW(NodeData(utils::parseIDFromHex(s)),
                        std::invalid_argument);
  }
}

BOOST_AUTO_TEST_CASE(too_long) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::string s;
    int length = rand() % TEST_LOOP_COUNT + 40 + 1;
    length += length % 2;
    s.assign(length, '1');
    BOOST_REQUIRE_THROW(NodeData(utils::parseIDFromHex(s)),
                        std::invalid_argument);
  }
}

BOOST_AUTO_TEST_CASE(comparing_fixed1) {
  NodeData a1(
      utils::parseIDFromHex("0F00000000000000000000000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("FF00000000000000000000000000000000000000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  BOOST_REQUIRE_EQUAL(a2 > a1, true);
  BOOST_REQUIRE_EQUAL(a2 >= a1, true);
  BOOST_REQUIRE_EQUAL(a2 < a1, false);
  BOOST_REQUIRE_EQUAL(a2 <= a1, false);
  BOOST_REQUIRE_EQUAL(a2 == a1, false);

  BOOST_REQUIRE_EQUAL(a3 > a2, true);
  BOOST_REQUIRE_EQUAL(a3 >= a2, true);
  BOOST_REQUIRE_EQUAL(a3 > a1, true);
  BOOST_REQUIRE_EQUAL(a3 >= a1, true);
  BOOST_REQUIRE_EQUAL(a3 < a2, false);
  BOOST_REQUIRE_EQUAL(a3 <= a2, false);
  BOOST_REQUIRE_EQUAL(a3 == a2, false);
  BOOST_REQUIRE_EQUAL(a3 < a1, false);
  BOOST_REQUIRE_EQUAL(a3 <= a1, false);
  BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(comparing_fixed2) {
  NodeData a1(
      utils::parseIDFromHex("0000000000000000000000000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("0000000000000000000F00000000000000000000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  BOOST_REQUIRE_EQUAL(a2 > a1, true);
  BOOST_REQUIRE_EQUAL(a2 >= a1, true);
  BOOST_REQUIRE_EQUAL(a2 < a1, false);
  BOOST_REQUIRE_EQUAL(a2 <= a1, false);
  BOOST_REQUIRE_EQUAL(a2 == a1, false);

  BOOST_REQUIRE_EQUAL(a3 > a2, true);
  BOOST_REQUIRE_EQUAL(a3 >= a2, true);
  BOOST_REQUIRE_EQUAL(a3 > a1, true);
  BOOST_REQUIRE_EQUAL(a3 >= a1, true);
  BOOST_REQUIRE_EQUAL(a3 < a2, false);
  BOOST_REQUIRE_EQUAL(a3 <= a2, false);
  BOOST_REQUIRE_EQUAL(a3 == a2, false);
  BOOST_REQUIRE_EQUAL(a3 < a1, false);
  BOOST_REQUIRE_EQUAL(a3 <= a1, false);
  BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(comparing_fixed3) {
  NodeData a1(
      utils::parseIDFromHex("000F000000000000000000000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("000F000000000000000F00000000000000000000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  BOOST_REQUIRE_EQUAL(a2 > a1, true);
  BOOST_REQUIRE_EQUAL(a2 >= a1, true);
  BOOST_REQUIRE_EQUAL(a2 < a1, false);
  BOOST_REQUIRE_EQUAL(a2 <= a1, false);
  BOOST_REQUIRE_EQUAL(a2 == a1, false);

  BOOST_REQUIRE_EQUAL(a3 > a2, true);
  BOOST_REQUIRE_EQUAL(a3 >= a2, true);
  BOOST_REQUIRE_EQUAL(a3 > a1, true);
  BOOST_REQUIRE_EQUAL(a3 >= a1, true);
  BOOST_REQUIRE_EQUAL(a3 < a2, false);
  BOOST_REQUIRE_EQUAL(a3 <= a2, false);
  BOOST_REQUIRE_EQUAL(a3 == a2, false);
  BOOST_REQUIRE_EQUAL(a3 < a1, false);
  BOOST_REQUIRE_EQUAL(a3 <= a1, false);
  BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(comparing_fixed4) {
  NodeData a1(
      utils::parseIDFromHex("000F000000000000000F00000000000000000000"));
  NodeData a2(
      utils::parseIDFromHex("000F000000000000000F00000000000000F00000"));
  NodeData a3(
      utils::parseIDFromHex("FFFF000000000000000000000000000000000000"));

  BOOST_REQUIRE_EQUAL(a2 > a1, true);
  BOOST_REQUIRE_EQUAL(a2 >= a1, true);
  BOOST_REQUIRE_EQUAL(a2 < a1, false);
  BOOST_REQUIRE_EQUAL(a2 <= a1, false);
  BOOST_REQUIRE_EQUAL(a2 == a1, false);

  BOOST_REQUIRE_EQUAL(a3 > a2, true);
  BOOST_REQUIRE_EQUAL(a3 >= a2, true);
  BOOST_REQUIRE_EQUAL(a3 > a1, true);
  BOOST_REQUIRE_EQUAL(a3 >= a1, true);
  BOOST_REQUIRE_EQUAL(a3 < a2, false);
  BOOST_REQUIRE_EQUAL(a3 <= a2, false);
  BOOST_REQUIRE_EQUAL(a3 == a2, false);
  BOOST_REQUIRE_EQUAL(a3 < a1, false);
  BOOST_REQUIRE_EQUAL(a3 <= a1, false);
  BOOST_REQUIRE_EQUAL(a3 == a1, false);
}

BOOST_AUTO_TEST_CASE(splitInHalf_ok) {
  NodeData low = NodeData::minValue;
  NodeData high = NodeData::maxValue;

  MaybeBounds bounds = NodeData::splitInHalf(low, high);

  BOOST_REQUIRE(!!bounds);
  BOOST_REQUIRE_LT(low, bounds->first);
  BOOST_REQUIRE_LT(bounds->first, bounds->second);
  BOOST_REQUIRE_LT(bounds->second, high);

  BOOST_REQUIRE_EQUAL(bounds->first.string(),
                      "7fffffffffffffffffffffffffffffffffffffff");
  BOOST_REQUIRE_EQUAL(bounds->second.string(),
                      "8000000000000000000000000000000000000000");

  // sub bounds low-bounds.
  MaybeBounds bounds_low = NodeData::splitInHalf(low, bounds->first);

  BOOST_REQUIRE(!!bounds_low);
  BOOST_REQUIRE_LT(low, bounds_low->first);
  BOOST_REQUIRE_LT(bounds_low->first, bounds_low->second);
  BOOST_REQUIRE_LT(bounds_low->second, bounds->first);

  BOOST_REQUIRE_EQUAL(bounds_low->first.string(),
                      "3fffffffffffffffffffffffffffffffffffffff");
  BOOST_REQUIRE_EQUAL(bounds_low->second.string(),
                      "4000000000000000000000000000000000000000");

  // sub bounds high-bounds.
  MaybeBounds bounds_high = NodeData::splitInHalf(bounds->second, high);

  BOOST_REQUIRE(!!bounds_high);
  BOOST_REQUIRE_LT(bounds->second, bounds_high->first);
  BOOST_REQUIRE_LT(bounds_high->first, bounds_high->second);
  BOOST_REQUIRE_LT(bounds_high->second, high);

  BOOST_REQUIRE_EQUAL(bounds_high->first.string(),
                      "bfffffffffffffffffffffffffffffffffffffff");
  BOOST_REQUIRE_EQUAL(bounds_high->second.string(),
                      "c000000000000000000000000000000000000000");
}

BOOST_AUTO_TEST_CASE(bytestring_parsing) {
  utils::Buffer s = {'1', '2', '3', '4', '5', '6', '7', '8', 'a', 'b',
                     'c', 'd', 'e', 'f', 'g', 'h', 'A', 'B', 'C', 'D'};
  NodeData d;
  d.read(s.begin(), s.cend());
  utils::Buffer b = d.write();
  BOOST_REQUIRE(b == s);
}

BOOST_AUTO_TEST_CASE(bytestring_parsing_special) {
  torrentsync::utils::Buffer s = {'1', '2', '3',  '\n', '5', '6', '7',
                                  '8', 'a', '\0', 'c',  'd', 'e', 'f',
                                  'g', 'h', '\0', 'B',  'C', 'D'};
  NodeData d;
  d.read(s.begin(), s.cend());
  torrentsync::utils::Buffer b = d.write();
  BOOST_REQUIRE(b == s);
}

BOOST_AUTO_TEST_SUITE_END();
