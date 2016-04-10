
#include <gtest/gtest.h>
#include <boost/foreach.hpp>
#include <torrentsync/dht/NodeTree.h>
#include <torrentsync/dht/NodeData.h>
#include <test/torrentsync/dht/CommonNodeTest.h>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

using namespace torrentsync;

namespace {
class NodeTreeF : public torrentsync::dht::NodeTree, public ::testing::Test {
public:
  NodeTreeF() : NodeTree(utils::parseIDFromHex(generateRandomNode())) {}
};
};

using namespace torrentsync::dht;
using namespace boost::assign;

TEST_F(NodeTreeF, addNode_oneSplit_toUp) {
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))));
  EXPECT_EQ(size(), 1);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))));
  EXPECT_EQ(size(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))));
  EXPECT_EQ(size(), 3);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))));
  EXPECT_EQ(size(), 4);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))));
  EXPECT_EQ(size(), 5);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))));
  EXPECT_EQ(size(), 6);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))));
  EXPECT_EQ(size(), 7);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))));
  EXPECT_EQ(size(), 8);
  EXPECT_EQ(getBucketsCount(), 1);

  EXPECT_THROW(addNode(NodeSPtr()), std::invalid_argument);

  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))));
  EXPECT_EQ(size(), 9);
  EXPECT_EQ(getBucketsCount(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))));
  EXPECT_EQ(size(), 10);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f6")))));
  EXPECT_EQ(size(), 11);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("04")))));
  EXPECT_EQ(size(), 12);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("05")))));
  EXPECT_EQ(size(), 13);
  EXPECT_EQ(getBucketsCount(), 2);
}

TEST_F(NodeTreeF, addNode_oneSplit_toLower) {
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("00")))));
  EXPECT_EQ(size(), 1);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("01")))));
  EXPECT_EQ(size(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("02")))));
  EXPECT_EQ(size(), 3);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("03")))));
  EXPECT_EQ(size(), 4);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f0")))));
  EXPECT_EQ(size(), 5);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f1")))));
  EXPECT_EQ(size(), 6);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f2")))));
  EXPECT_EQ(size(), 7);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f3")))));
  EXPECT_EQ(size(), 8);
  EXPECT_EQ(getBucketsCount(), 1);

  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("04")))));
  EXPECT_EQ(size(), 9);
  EXPECT_EQ(getBucketsCount(), 2);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("05")))));
  EXPECT_EQ(size(), 10);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("06")))));
  EXPECT_EQ(size(), 11);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f4")))));
  EXPECT_EQ(size(), 12);
  addNode(NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode("f5")))));
  EXPECT_EQ(size(), 13);
  EXPECT_EQ(getBucketsCount(), 2);
}

TEST_F(NodeTreeF, addNode_inARow_000_to_999) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << a << b << c;
        EXPECT_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

TEST_F(NodeTreeF, addNode_inARow_999_to_000) {
  for (int a = 9; a >= 0; --a) {
    for (int b = 9; b >= 0; --b) {
      for (int c = 9; c >= 0; --c) {
        std::stringstream num;
        num << a << b << c;
        EXPECT_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

TEST_F(NodeTreeF, addNode_inARow_000_to_999_cba) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << c << b << a;
        EXPECT_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

TEST_F(NodeTreeF, addNode_inARow_000_to_999_bca) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << b << c << a;
        EXPECT_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

TEST_F(NodeTreeF, addNode_inARow_000_to_999_acb) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << a << c << b;
        EXPECT_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

TEST_F(NodeTreeF, addNode_inARow_000_to_999_cab) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << c << a << b;
        EXPECT_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

TEST_F(NodeTreeF, addNode_inARow_000_to_999_bac) {
  for (int a = 0; a < 9; ++a) {
    for (int b = 0; b < 9; ++b) {
      for (int c = 0; c < 9; ++c) {
        std::stringstream num;
        num << b << a << c;
        EXPECT_NO_THROW(addNode(NodeSPtr(
            new Node(utils::parseIDFromHex(generateRandomNode(num.str()))))));
      }
    }
  }
}

TEST_F(NodeTreeF, addNode_someRandom) {
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
    EXPECT_EQ(this->size(), 0);
  }
}

TEST_F(NodeTreeF, addNode_and_find) {
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
      EXPECT_NO_THROW(addNode(addr));
    });

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      boost::optional<NodeSPtr> b = getNode(*addr);
      EXPECT_TRUE(!!b);
      EXPECT_NE(b.get(), nullptr);
      EXPECT_EQ(**b, *addr);
    });

    for (int j = 0; j < TEST_LOOP_COUNT; ++j) {
      boost::optional<NodeSPtr> b =
          getNode(utils::parseIDFromHex(generateRandomNode("b")));
      EXPECT_TRUE(!b);
    }

    clear();
  }
}

TEST_F(NodeTreeF, removeNode_fail) {
  NodeSPtr ptr;
  EXPECT_THROW(removeNode(ptr), std::invalid_argument);
}

TEST_F(NodeTreeF, removeNode_removeSerial) {
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
      EXPECT_NO_THROW(addNode(addr));
    });
    EXPECT_EQ(size(), v.size());

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr addr) {
      EXPECT_NO_THROW(removeNode(addr));
    });

    EXPECT_EQ(size(), 0);
  }
}

TEST_F(NodeTreeF, removeNode_removeRandomOrder) {
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
      EXPECT_NO_THROW(addNode(addr));
    });

    while (!v.empty()) {
      const int idx = (rand() + 1) % v.size();
      EXPECT_NO_THROW(removeNode(v[idx]));
      v.erase(v.begin() + idx);
    }

    EXPECT_EQ(size(), 0);
  }
}

TEST_F(NodeTreeF, removeNode_addAndRemove) {
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
      EXPECT_NO_THROW(addNode(a));
    });
    EXPECT_EQ(size(), v.size());

    NodeSPtr a(new Node(utils::parseIDFromHex(generateRandomNode("04"))));
    NodeSPtr b(new Node(utils::parseIDFromHex(generateRandomNode("05"))));

    const int numberToRemove = 2;
    for (int j = 0; j < numberToRemove; ++j) {
      const int idx = (rand() + 1) % v.size();
      EXPECT_NO_THROW(removeNode(v[idx]));
      v.erase(v.begin() + idx);
    }
    EXPECT_EQ(size(), v.size());
    EXPECT_NO_THROW(addNode(a));
    EXPECT_NO_THROW(addNode(b));
    EXPECT_EQ(size(), v.size() + numberToRemove);

    std::for_each(v.begin(), v.end(), [&](const NodeSPtr a) {
      EXPECT_NO_THROW(removeNode(a));
    });

    EXPECT_EQ(size(), numberToRemove);
    EXPECT_NO_THROW(removeNode(a));
    EXPECT_NO_THROW(removeNode(b));
    EXPECT_EQ(size(), 0);
  }
}

TEST_F(NodeTreeF, getClosestNode_1) {
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
      EXPECT_NO_THROW(addNode(addr));
    });
    EXPECT_EQ(size(), v.size());

    auto nodes = getClosestNodes(utils::parseIDFromHex(generateRandomNode()));
    EXPECT_LE(nodes.size(), DHT_FIND_NODE_COUNT);
    clear();
  }
}

TEST_F(NodeTreeF, getClosestNode_2) {
  for (size_t _i = 0; _i < TEST_LOOP_COUNT; ++_i) {
    std::vector<NodeSPtr> v;
    for (size_t _t = 0; _t < 1000; ++_t) {
      NodeSPtr n =
          std::make_shared<Node>(utils::parseIDFromHex(generateRandomNode()));
      if (addNode(n))
        v += n;
    }
    EXPECT_EQ(size(), v.size());

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
      ASSERT_TRUE(std::find_if(min, max, [&](const NodeSPtr &p) -> bool {
                      return *p == *n;
                    }) != v.end());
    };
    EXPECT_LE(nodes.size(), DHT_FIND_NODE_COUNT);

    clear();
  }
}

TEST_F(NodeTreeF, getClosestNode_perfectmatch) {
  for (size_t _i = 0; _i < TEST_LOOP_COUNT; ++_i) {
    std::vector<NodeSPtr> v;
    for (size_t _t = 0; _t < 1000; ++_t) {
      NodeSPtr n =
          NodeSPtr(new Node(utils::parseIDFromHex(generateRandomNode())));
      if (addNode(n))
        v += n;
    }
    EXPECT_EQ(size(), v.size());

    // take a random address and get the close nodes
    auto nodes = getClosestNodes(*v[rand() % v.size()]);
    ASSERT_LE(nodes.size(), 1);

    clear();
  }
}


