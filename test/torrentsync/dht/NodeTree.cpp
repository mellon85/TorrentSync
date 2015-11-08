
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
#include <torrentsync/dht/NodeTree.h>
#include <torrentsync/dht/NodeData.h>
#include <test/torrentsync/dht/CommonNodeTest.h>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

using namespace torrentsync;

namespace {
class NodeTreeFixture : public torrentsync::dht::NodeTree {
public:
  NodeTreeFixture() : NodeTree(utils::parseIDFromHex(generateRandomNode())) {}
};
};

BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_NodeTree, NodeTreeFixture);

using namespace torrentsync::dht;
using namespace boost::assign;

BOOST_AUTO_TEST_CASE(addNode_oneSplit_toUp) {
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))));
  BOOST_REQUIRE_EQUAL(size(), 1);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))));
  BOOST_REQUIRE_EQUAL(size(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))));
  BOOST_REQUIRE_EQUAL(size(), 3);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))));
  BOOST_REQUIRE_EQUAL(size(), 4);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))));
  BOOST_REQUIRE_EQUAL(size(), 5);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))));
  BOOST_REQUIRE_EQUAL(size(), 6);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))));
  BOOST_REQUIRE_EQUAL(size(), 7);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))));
  BOOST_REQUIRE_EQUAL(size(), 8);
  BOOST_REQUIRE_EQUAL(getBucketsCount(), 1);

  BOOST_REQUIRE_THROW(addNode(NodeSPtr()), std::invalid_argument);

  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))));
  BOOST_REQUIRE_EQUAL(size(), 9);
  BOOST_REQUIRE_EQUAL(getBucketsCount(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))));
  BOOST_REQUIRE_EQUAL(size(), 10);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f6")))));
  BOOST_REQUIRE_EQUAL(size(), 11);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("04")))));
  BOOST_REQUIRE_EQUAL(size(), 12);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("05")))));
  BOOST_REQUIRE_EQUAL(size(), 13);
  BOOST_REQUIRE_EQUAL(getBucketsCount(), 2);
}

BOOST_AUTO_TEST_CASE(addNode_oneSplit_toLower) {
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))));
  BOOST_REQUIRE_EQUAL(size(), 1);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))));
  BOOST_REQUIRE_EQUAL(size(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))));
  BOOST_REQUIRE_EQUAL(size(), 3);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))));
  BOOST_REQUIRE_EQUAL(size(), 4);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))));
  BOOST_REQUIRE_EQUAL(size(), 5);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))));
  BOOST_REQUIRE_EQUAL(size(), 6);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))));
  BOOST_REQUIRE_EQUAL(size(), 7);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))));
  BOOST_REQUIRE_EQUAL(size(), 8);
  BOOST_REQUIRE_EQUAL(getBucketsCount(), 1);

  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("04")))));
  BOOST_REQUIRE_EQUAL(size(), 9);
  BOOST_REQUIRE_EQUAL(getBucketsCount(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("05")))));
  BOOST_REQUIRE_EQUAL(size(), 10);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("06")))));
  BOOST_REQUIRE_EQUAL(size(), 11);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))));
  BOOST_REQUIRE_EQUAL(size(), 12);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))));
  BOOST_REQUIRE_EQUAL(size(), 13);
  BOOST_REQUIRE_EQUAL(getBucketsCount(), 2);
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << a << b << c;
        BOOST_REQUIRE_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_999_to_000) {
  for (int a = 9; a >= 0; --a) {
    for (int b = 9; b >= 0; --b) {
      for (int c = 9; c >= 0; --c) {
        std::stringstream num;
        num << a << b << c;
        BOOST_REQUIRE_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_cba) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << c << b << a;
        BOOST_REQUIRE_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_bca) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << b << c << a;
        BOOST_REQUIRE_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_acb) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << a << c << b;
        BOOST_REQUIRE_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_cab) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << c << a << b;
        BOOST_REQUIRE_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_bac) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << b << a << c;
        BOOST_REQUIRE_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

BOOST_AUTO_TEST_CASE(addNode_someRandom) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<NodeSPtr> addresses;
    const size_t count = (rand() + 1) % ((rand() % 4096) + 1);
    for (size_t j = 0; j < count; j++) {
      bool bad = false;
      NodeSPtr a(new Node(utils::parseIDFromHex(generateRandomNode())));
      std::for_each(addresses.begin(), addresses.end(),
                    [&](const NodeSPtr &it) {
                      if (*a == *it) {
                        --j;
                        bad = true;
                        return;
                      }
                    });
      if (!bad) {
        addresses.push_back(a);
        addNode(a);
      }
    }
    this->clear();
    BOOST_REQUIRE_EQUAL(this->size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(addNode_and_find) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<NodeSPtr> v;
    v += NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f6")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("04")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("05"))));

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      BOOST_REQUIRE_NO_THROW(addNode(addr));
    });

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      boost::optional<NodeSPtr> b = getNode(*addr);
      BOOST_CHECK(!!b);
      BOOST_REQUIRE(b.get());
      BOOST_CHECK_EQUAL(**b, *addr);
    });

    for (int j = 0; j < TEST_LOOP_COUNT; ++j) {
      boost::optional<NodeSPtr> b =
          getNode(utils::parseIDFromHex(generateRandomNode("b")));
      BOOST_CHECK(!b);
    }

    clear();
  }
}

BOOST_AUTO_TEST_CASE(removeNode_fail) {
  NodeSPtr ptr;
  BOOST_REQUIRE_THROW(removeNode(ptr), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(removeNode_removeSerial) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<NodeSPtr> v;
    v += NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f6")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("04")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("05"))));

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      BOOST_REQUIRE_NO_THROW(addNode(addr));
    });
    BOOST_REQUIRE_EQUAL(size(), v.size());

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      BOOST_REQUIRE_NO_THROW(removeNode(addr));
    });

    BOOST_REQUIRE_EQUAL(size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(removeNode_removeRandomOrder) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<NodeSPtr> v;
    v += NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f6")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("04")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("05"))));

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      BOOST_REQUIRE_NO_THROW(addNode(addr));
    });

    while (!v.empty()) {
      const int idx = (rand() + 1) % v.size();
      BOOST_REQUIRE_NO_THROW(removeNode(v[idx]));
      v.erase(v.begin() + idx);
    }

    BOOST_REQUIRE_EQUAL(size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(removeNode_addAndRemove) {
  std::vector<NodeSPtr> v;

  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<NodeSPtr> v;
    v += NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f6"))));

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr a) {
      BOOST_REQUIRE_NO_THROW(addNode(a));
    });
    BOOST_REQUIRE_EQUAL(size(), v.size());

    NodeSPtr a(new Node(utils::parseIDFromHex(generateRandomNode("04"))));
    NodeSPtr b(new Node(utils::parseIDFromHex(generateRandomNode("05"))));

    const int numberToRemove = 2;
    for (int j = 0; j < numberToRemove; ++j) {
      const int idx = (rand() + 1) % v.size();
      BOOST_REQUIRE_NO_THROW(removeNode(v[idx]));
      v.erase(v.begin() + idx);
    }
    BOOST_REQUIRE_EQUAL(size(), v.size());
    BOOST_REQUIRE_NO_THROW(addNode(a));
    BOOST_REQUIRE_NO_THROW(addNode(b));
    BOOST_REQUIRE_EQUAL(size(), v.size() + numberToRemove);

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr a) {
      BOOST_REQUIRE_NO_THROW(removeNode(a));
    });

    BOOST_REQUIRE_EQUAL(size(), numberToRemove);
    BOOST_REQUIRE_NO_THROW(removeNode(a));
    BOOST_REQUIRE_NO_THROW(removeNode(b));
    BOOST_REQUIRE_EQUAL(size(), 0);
  }
}

BOOST_AUTO_TEST_CASE(getClosestNode_1) {
  for (int i = 0; i < TEST_LOOP_COUNT; ++i) {
    std::vector<NodeSPtr> v;
    v += NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))),
        NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2"))));

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      BOOST_REQUIRE_NO_THROW(addNode(addr));
    });
    BOOST_REQUIRE_EQUAL(size(), v.size());

    auto nodes = getClosestNodes(utils::parseIDFromHex(generateRandomNode()));
    BOOST_REQUIRE_LE(nodes.size(), DHT_FIND_NODE_COUNT);
    clear();
  }
}

BOOST_AUTO_TEST_CASE(getClosestNode_2) {
  for (size_t _i = 0; _i < TEST_LOOP_COUNT; ++_i) {
    std::vector<NodeSPtr> v;
    for (size_t _t = 0; _t < 1000; ++_t) {
      NodeSPtr n =
          std::make_shared<Node>(utils::parseIDFromHex(generateRandomNode()));
      if (addNode(n))
        v += n;
    }
    BOOST_REQUIRE_EQUAL(size(), v.size());

    // take a random address and get the close nodes
    Node addr = utils::parseIDFromHex(generateRandomNode());
    auto nodes = getClosestNodes(addr);

    // sort the nodes for easy search
    std::sort(v.begin(), v.end(),
              [](const NodeSPtr &a, const NodeSPtr &b) { return *a <= *b; });

    // verify that the result is nodes from the closest nodes
    auto it = v.begin();
    while (it != v.end() && **it < addr)
      ++it;

    const size_t distance_to_end = v.end() - it;
    const size_t distance_to_begin = it - v.begin();
    auto min =
        it - std::min(distance_to_begin, std::max((size_t)12, distance_to_end));
    auto max =
        it + std::min(distance_to_end, std::max((size_t)12, distance_to_begin));

    // all elements of nodes should be in this interval
    for (const auto &n : nodes) {
      BOOST_REQUIRE(std::find_if(min, max, [&](const NodeSPtr &p) -> bool {
                      return *p == *n;
                    }) != v.end());
    };
    BOOST_REQUIRE_LE(nodes.size(), DHT_FIND_NODE_COUNT);

    clear();
  }
}

BOOST_AUTO_TEST_CASE(getClosestNode_perfectmatch) {
  for (size_t _i = 0; _i < TEST_LOOP_COUNT; ++_i) {
    std::vector<NodeSPtr> v;
    for (size_t _t = 0; _t < 1000; ++_t) {
      NodeSPtr n =
          NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode())));
      if (addNode(n))
        v += n;
    }
    BOOST_REQUIRE_EQUAL(size(), v.size());

    // take a random address and get the close nodes
    auto nodes = getClosestNodes(*v[rand() % v.size()]);
    BOOST_REQUIRE_LE(nodes.size(), 1);

    clear();
  }
}

BOOST_AUTO_TEST_SUITE_END();
