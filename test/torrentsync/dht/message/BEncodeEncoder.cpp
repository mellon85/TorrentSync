#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

#include <torrentsync/dht/message/BEncodeEncoder.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_BEncodeEncoder);

using namespace torrentsync;
using namespace torrentsync::dht::message;

BOOST_AUTO_TEST_CASE(constructor_destructor) { BEncodeEncoder enc; }

BOOST_AUTO_TEST_CASE(check_lexicographic_order) {
  BEncodeEncoder c;

  c.startDictionary();
  c.addDictionaryElement("a", "a");
  c.addDictionaryElement("c", "c");
  BOOST_REQUIRE_THROW(c.addDictionaryElement("b", "b"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(add_integer) {
  BEncodeEncoder c;

  c.startList();
  c.addInteger(12);
  c.endList();

  BOOST_REQUIRE(c.value() == utils::makeBuffer("li12ee"));
}

BOOST_AUTO_TEST_CASE(bep005_ping_example_q) {
  BEncodeEncoder c;

  c.startDictionary();
  c.addElement("a");
  c.startDictionary();
  c.addDictionaryElement("id", "abcdefghij0123456789");
  c.endDictionary();
  c.addDictionaryElement("q", "ping");
  c.addDictionaryElement("t", "aa");
  c.addDictionaryElement("y", "q");
  c.endDictionary();

  BOOST_REQUIRE(
      c.value() ==
      utils::makeBuffer(
          "d1:ad2:id20:abcdefghij0123456789e1:q4:ping1:t2:aa1:y1:qe"));
}

BOOST_AUTO_TEST_CASE(bep005_ping_example_r) {
  BEncodeEncoder c;

  c.startDictionary();
  c.addElement("r");
  c.startDictionary();
  c.addDictionaryElement("id", "mnopqrstuvwxyz123456");
  c.endDictionary();
  c.addDictionaryElement("t", "aa");
  c.addDictionaryElement("y", "r");
  c.endDictionary();

  BOOST_REQUIRE(
      c.value() ==
      utils::makeBuffer("d1:rd2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:re"));
}

BOOST_AUTO_TEST_CASE(bep005_find_node_example_q) {
  BEncodeEncoder c;

  c.startDictionary();
  c.addElement("a");
  c.startDictionary();
  c.addDictionaryElement("id", "abcdefghij0123456789");
  c.addDictionaryElement("target", "mnopqrstuvwxyz123456");
  c.endDictionary();
  c.addDictionaryElement("q", "find_node");
  c.addDictionaryElement("t", "aa");
  c.addDictionaryElement("y", "q");
  c.endDictionary();

  BOOST_REQUIRE(
      c.value() ==
      utils::makeBuffer("d1:ad2:id20:abcdefghij01234567896:target20:"
                        "mnopqrstuvwxyz123456e1:q9:find_node1:t2:aa1:y1:qe"));
}

BOOST_AUTO_TEST_CASE(bep005_find_node_example_r) {
  BEncodeEncoder c;

  c.startDictionary();
  c.addElement("r");
  c.startDictionary();
  c.addDictionaryElement("id", "0123456789abcdefghij");
  c.addDictionaryElement("nodes", "0123456789abcdefghij....11");
  c.endDictionary();
  c.addDictionaryElement("t", "aa");
  c.addDictionaryElement("y", "r");
  c.endDictionary();

  BOOST_REQUIRE(c.value() ==
                utils::makeBuffer("d1:rd2:id20:0123456789abcdefghij5:nodes26:"
                                  "0123456789abcdefghij....11e1:t2:aa1:y1:re"));
}

BOOST_AUTO_TEST_SUITE_END();
