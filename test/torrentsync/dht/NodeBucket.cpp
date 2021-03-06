
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/bind.hpp>

#include <cstdlib>
#include <sstream>

#include <torrentsync/dht/NodeBucket.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

namespace std {
template <size_t Size> struct less<torrentsync::dht::NodeBucket<Size>> {
  bool operator()(const torrentsync::dht::NodeBucket<Size> &x,
                  const torrentsync::dht::NodeBucket<Size> &y) const {
    return x.getUpperBound() < y.getLowerBound();
  }
};
};

BOOST_AUTO_TEST_SUITE(torrentsync_dht_NodeBucket);

using namespace torrentsync::dht;
using namespace torrentsync;
using namespace boost::assign;

BOOST_AUTO_TEST_CASE(constructor) {
  NodeData lowbound = NodeData(
      utils::parseIDFromHex("0000000000000000000000000000000000000000"));
  NodeData highbound = NodeData(
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"));
  NodeBucket<8> bucket(lowbound, highbound);

  BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
  BOOST_REQUIRE(bucket.getUpperBound() == highbound);
  BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  BOOST_REQUIRE_EQUAL(bucket.maxSize(), 8);

  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    const std::string str = generateRandomNode();
    std::shared_ptr<Node> addr(new Node(utils::parseIDFromHex(str)));
    BOOST_REQUIRE_EQUAL(true, bucket.inBounds(addr));
  }

  BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
  BOOST_REQUIRE(bucket.getUpperBound() == highbound);
  BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  BOOST_REQUIRE_EQUAL(bucket.maxSize(), 8);
  bucket.clear();
}

BOOST_AUTO_TEST_CASE(outside) {
  NodeData lowbound = NodeData(
      utils::parseIDFromHex("0000000000000000000000000000000000000000"));
  NodeData highbound = NodeData(
      utils::parseIDFromHex("0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"));
  NodeBucket<8> bucket(lowbound, highbound);

  BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
  BOOST_REQUIRE(bucket.getUpperBound() == highbound);
  BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  BOOST_REQUIRE_EQUAL(bucket.maxSize(), 8);

  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::string str = generateRandomNode("F");
    std::shared_ptr<Node> addr(new Node(utils::parseIDFromHex(str)));
    BOOST_REQUIRE_EQUAL(false, bucket.inBounds(addr));
  }

  BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
  BOOST_REQUIRE(bucket.getUpperBound() == highbound);
  BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  BOOST_REQUIRE_EQUAL(bucket.maxSize(), 8);
}

BOOST_AUTO_TEST_CASE(inside) {
  NodeData lowbound = NodeData(
      utils::parseIDFromHex("0000000000000000000000000000000000000000"));
  NodeData highbound = NodeData(
      utils::parseIDFromHex("000002003400FFFFFFFFFFFFFFFFFFFFFFFFFFFF"));
  NodeBucket<2> bucket(lowbound, highbound);

  BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
  BOOST_REQUIRE(bucket.getUpperBound() == highbound);
  BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  BOOST_REQUIRE_EQUAL(bucket.maxSize(), 2);

  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::string str = generateRandomNode("000002003400");
    std::shared_ptr<Node> addr(new Node(utils::parseIDFromHex(str)));
    BOOST_REQUIRE_EQUAL(true, bucket.inBounds(addr));
  }

  BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
  BOOST_REQUIRE(bucket.getUpperBound() == highbound);
  BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  BOOST_REQUIRE_EQUAL(bucket.maxSize(), 2);
}

BOOST_AUTO_TEST_CASE(bucket_ordering) {
  NodeData a1 =
      utils::parseIDFromHex("0000000000000000000000000000000000000000");
  NodeData a2 =
      utils::parseIDFromHex("000002003400FFFFFFFFFFFFFFFFFFFFFFFFFFFF");
  NodeData a3 =
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
  NodeData a2_1 =
      utils::parseIDFromHex("0000020034010000000000000000000000000000");

  BOOST_REQUIRE(a1 <= a2);
  BOOST_REQUIRE(a2 <= a3);

  NodeBucket<3> b1(a1, a2);
  NodeBucket<3> b2(a2_1, a3);

  BOOST_REQUIRE(std::less<NodeBucket<3>>()(b1, b2));
}

BOOST_AUTO_TEST_CASE(add_remove) {
  NodeData bot =
      utils::parseIDFromHex("0000000000000000000000000000000000000000");
  NodeData top =
      utils::parseIDFromHex("0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    NodeBucket<10> bucket(bot, top);

    std::vector<std::shared_ptr<Node>> addresses;

    for (int i = 0; i < 10; ++i) {
      std::shared_ptr<Node> a = std::shared_ptr<Node>(
          new Node(utils::parseIDFromHex(generateRandomNode("0"))));
      addresses.push_back(a);
      BOOST_REQUIRE_NO_THROW(BOOST_REQUIRE(bucket.add(a)));
      std::shared_ptr<Node> f = std::shared_ptr<Node>(
          new Node(utils::parseIDFromHex(generateRandomNode("F"))));
      BOOST_REQUIRE_THROW(bucket.add(f), std::invalid_argument);

      std::for_each(addresses.begin(), addresses.end(),
                    [&](const std::shared_ptr<Node> &va) {
                      BOOST_REQUIRE(std::find(bucket.cbegin(), bucket.cend(),
                                              va) != bucket.cend());
                    });
      BOOST_REQUIRE_EQUAL(bucket.size(), i + 1);
    }
    std::shared_ptr<Node> a = std::shared_ptr<Node>(
        new Node(utils::parseIDFromHex(generateRandomNode("0"))));
    BOOST_REQUIRE_EQUAL(false, bucket.add(a));
    BOOST_REQUIRE_EQUAL(bucket.size(), 10);

    std::for_each(addresses.begin(), addresses.end(),
                  [&](const std::shared_ptr<Node> &va) {
                    BOOST_REQUIRE(std::find(bucket.cbegin(), bucket.cend(),
                                            va) != bucket.cend());
                  });

    for (int i = addresses.size(); i > 0; --i) {
      BOOST_REQUIRE(addresses.size() > 0);
      const int index = rand() % addresses.size();
      const int start_size = addresses.size();
      std::shared_ptr<Node> a = addresses[index];
      addresses.erase(addresses.begin() + index);
      BOOST_REQUIRE(a.get() > 0);

      BOOST_REQUIRE_NO_THROW(BOOST_REQUIRE(bucket.remove(*a)));
      BOOST_REQUIRE_EQUAL(bucket.size(), start_size - 1);

      std::for_each(addresses.begin(), addresses.end(),
                    [&](const std::shared_ptr<Node> &va) {
                      BOOST_REQUIRE(std::find(bucket.cbegin(), bucket.cend(),
                                              va) != bucket.cend());
                    });
      BOOST_REQUIRE(std::find(bucket.cbegin(), bucket.cend(), a) ==
                    bucket.cend());
    }

    BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  }
}

class FakeNode : public Node {
public:
  FakeNode(const std::string &str) : Node(utils::parseIDFromHex(str)) {}

  time_t &getTime() { return Node::_last_time_good; }
  auto getLastUnansweredQueries() -> decltype(Node::_last_unanswered_queries) {
    return Node::_last_unanswered_queries;
  }

  void setLastUnansweredQueries(uint8_t v) { Node::_last_unanswered_queries = v; }
};

BOOST_AUTO_TEST_CASE(removeBad) {
  NodeData bot =
      utils::parseIDFromHex("0000000000000000000000000000000000000000");
  NodeData top =
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<std::shared_ptr<Node>> addresses;

    for (int i = 0; i < 10; ++i) {
      std::shared_ptr<Node> a =
          std::shared_ptr<Node>(new FakeNode(generateRandomNode()));
      addresses.push_back(a);
    }

    for (int j = 0; j < TEST_LOOP_COUNT; ++j) {
      NodeBucket<10> bucket(bot, top);
      std::for_each(addresses.begin(), addresses.end(),
                    [&](std::shared_ptr<Node> &a) { // deep copy into bucket
                      std::shared_ptr<Node> new_a(new Node(*a));
                      BOOST_REQUIRE_NO_THROW(BOOST_REQUIRE(bucket.add(new_a)));
                    });

      const int setbad_count = rand() % bucket.size();
      const int size = bucket.size();

      int setbad = 0;
      for (int bad = 0; bad < setbad_count; ++bad) {
        std::shared_ptr<Node> a = *(bucket.cbegin() + rand() % bucket.size());
        FakeNode *af = reinterpret_cast<FakeNode *>(a.get());
        if (af->getTime() > 0)
          ++setbad;
        af->getTime() = 0;
        af->setLastUnansweredQueries(
            torrentsync::dht::Node::allowed_unanswered_queries + 1);
        BOOST_REQUIRE(af->isBad());
      }
      BOOST_REQUIRE_NO_THROW(bucket.removeBad());
      BOOST_REQUIRE_EQUAL(bucket.size(), size - setbad);
    }
  }
}

BOOST_AUTO_TEST_CASE(removeBad_2) {
  NodeData bot = NodeData(
      utils::parseIDFromHex("0000000000000000000000000000000000000000"));
  NodeData top = NodeData(
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"));

  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<std::shared_ptr<Node>> addresses;

    for (int i = 0; i < 10; ++i) {
      std::shared_ptr<Node> a =
          std::shared_ptr<Node>(new FakeNode(generateRandomNode()));
      addresses.push_back(a);
    }

    NodeBucket<10> bucket(bot, top);
    while (bucket.size() > 0) {
      std::for_each(addresses.begin(), addresses.end(),
                    [&](std::shared_ptr<Node> &a) { // deep copy into bucket
                      std::shared_ptr<Node> new_a(new Node(*a));
                      BOOST_REQUIRE_NO_THROW(BOOST_REQUIRE(bucket.add(new_a)));
                    });

      const int setbad_count = rand() % bucket.size();
      const int size = bucket.size();

      int setbad = 0;
      for (int bad = 0; bad < setbad_count; ++bad) {
        std::shared_ptr<Node> a = *(bucket.cbegin() + rand() % bucket.size());
        FakeNode *af = reinterpret_cast<FakeNode *>(a.get());
        if (af->getTime() > 0)
          ++setbad;
        af->getTime() = 0;
        af->setLastUnansweredQueries(
            torrentsync::dht::Node::allowed_unanswered_queries + 1);
        BOOST_REQUIRE(af->isBad());
      }
      BOOST_REQUIRE_NO_THROW(bucket.removeBad());
      BOOST_REQUIRE_EQUAL(bucket.size(), size - setbad);
    }
    BOOST_REQUIRE_EQUAL(bucket.size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(addIsSorted) {
  NodeData bot =
      utils::parseIDFromHex("0000000000000000000000000000000000000000");
  NodeData top =
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  std::vector<Node> addresses;
  addresses +=
      utils::parseIDFromHex("0000000000000000000000000000000000000001"),
      utils::parseIDFromHex("0000000000000000000000000000000000000002"),
      utils::parseIDFromHex("00000000000000000000000000F0000000000000"),
      utils::parseIDFromHex("00000000000000000000F0000000000000000000"),
      utils::parseIDFromHex("0000000000000000F00000000000000000000000");

  std::vector<Node> sorted_addresses = addresses;

  do {
    NodeBucket<10> bucket(bot, top);
    std::for_each(addresses.begin(), addresses.end(), [&](const Node &addr) {
      std::shared_ptr<Node> a(new Node(addr));
      bucket.add(a);
    });
    std::vector<Node>::iterator it2 = sorted_addresses.begin();
    for (NodeBucket<10>::const_iterator it = bucket.cbegin();
         it != bucket.cend(); ++it, ++it2) {
      BOOST_REQUIRE_EQUAL(**it, *it2);
    }
  } while (std::next_permutation(addresses.begin(), addresses.end()));
}

BOOST_AUTO_TEST_CASE(addIsSortedRandom) {
  NodeData bot =
      utils::parseIDFromHex("0000000000000000000000000000000000000000");
  NodeData top =
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  for (int loop = 0; loop < TEST_LOOP_COUNT; ++loop) {
    std::vector<Node> addresses;
    for (int i = 0; i < 10; ++i) {
      addresses.push_back(utils::parseIDFromHex(generateRandomNode()));
    }
    std::vector<Node> sorted_addresses = addresses;
    std::sort(sorted_addresses.begin(), sorted_addresses.end());

    NodeBucket<10> bucket(bot, top);
    std::for_each(addresses.begin(), addresses.end(), [&](const Node &addr) {
      std::shared_ptr<Node> a(new Node(addr));
      bucket.add(a);
    });
    std::vector<Node>::iterator it2 = sorted_addresses.begin();
    for (NodeBucket<10>::const_iterator it = bucket.cbegin();
         it != bucket.cend(); ++it, ++it2) {
      BOOST_REQUIRE_EQUAL(**it, *it2);
    }
  }
}

BOOST_AUTO_TEST_CASE(add_and_find) {
  NodeData bot =
      utils::parseIDFromHex("0000000000000000000000000000000000000000");
  NodeData top =
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  std::vector<Node> addresses;
  addresses +=
      utils::parseIDFromHex("0000000000000000000000000000000000000001"),
      utils::parseIDFromHex("0000000000000000000000000000000000000002"),
      utils::parseIDFromHex("00000000000000000000000000F0000000000000"),
      utils::parseIDFromHex("00000000000000000000F0000000000000000000"),
      utils::parseIDFromHex("0000000000000000F00000000000000000000000");

  std::vector<Node> sorted_addresses = addresses;

  do {
    NodeBucket<10> bucket(bot, top);
    std::for_each(addresses.begin(), addresses.end(), [&](const Node &addr) {
      std::shared_ptr<Node> a(new Node(addr));
      bucket.add(a);
    });
    std::for_each(addresses.begin(), addresses.end(), [&](const Node &addr) {
      boost::optional<NodeSPtr> b = bucket.find(addr);
      BOOST_CHECK(!!b);
      BOOST_REQUIRE(b.get());
      BOOST_REQUIRE_EQUAL(**b, addr);
    });
    for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
      boost::optional<NodeSPtr> b =
          bucket.find(utils::parseIDFromHex(generateRandomNode("F")));
      BOOST_CHECK(!b);
    }
  } while (std::next_permutation(addresses.begin(), addresses.end()));
}

BOOST_AUTO_TEST_CASE(addRandom_and_find) {
  NodeData bot =
      utils::parseIDFromHex("0000000000000000000000000000000000000000");
  NodeData top =
      utils::parseIDFromHex("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

  for (int loop = 0; loop < TEST_LOOP_COUNT; ++loop) {
    std::vector<Node> addresses;
    for (int i = 0; i < 10; ++i) {
      addresses.push_back(utils::parseIDFromHex(generateRandomNode()));
    }
    std::vector<Node> sorted_addresses = addresses;
    std::sort(sorted_addresses.begin(), sorted_addresses.end());

    NodeBucket<10> bucket(bot, top);
    std::for_each(addresses.begin(), addresses.end(), [&](const Node &addr) {
      std::shared_ptr<Node> a(new Node(addr));
      bucket.add(a);
    });
    std::for_each(addresses.begin(), addresses.end(), [&](const Node &addr) {
      boost::optional<NodeSPtr> b = bucket.find(addr);
      BOOST_CHECK(!!b);
      BOOST_REQUIRE(b.get());
      BOOST_CHECK_EQUAL(**b, addr);
    });
    for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
      boost::optional<NodeSPtr> b =
          bucket.find(utils::parseIDFromHex(generateRandomNode("F")));
      BOOST_CHECK(!b);
    }
  }
}

BOOST_AUTO_TEST_SUITE_END();
