
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
#include <torrentsync/dht/NodeTree.h>
#include <test/torrentsync/dht/CommonNodeTest.h>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

namespace
{
class NodeTreeFixture : public torrentsync::dht::NodeTree
{
public:
    NodeTreeFixture() : NodeTree(
        torrentsync::dht::NodeData::parse(generateRandomNode())) {}
};
};

BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_NodeTree,NodeTreeFixture);

using namespace torrentsync::dht;
using namespace boost::assign;

BOOST_AUTO_TEST_CASE(addNode_oneSplit_toUp)
{
    addNode(NodeSPtr(new Node(generateRandomNode("00"))));
    BOOST_REQUIRE_EQUAL(size(),1);
    addNode(NodeSPtr(new Node(generateRandomNode("01"))));
    BOOST_REQUIRE_EQUAL(size(),2);
    addNode(NodeSPtr(new Node(generateRandomNode("02"))));
    BOOST_REQUIRE_EQUAL(size(),3);
    addNode(NodeSPtr(new Node(generateRandomNode("03"))));
    BOOST_REQUIRE_EQUAL(size(),4);
    addNode(NodeSPtr(new Node(generateRandomNode("f0"))));
    BOOST_REQUIRE_EQUAL(size(),5);
    addNode(NodeSPtr(new Node(generateRandomNode("f1"))));
    BOOST_REQUIRE_EQUAL(size(),6);
    addNode(NodeSPtr(new Node(generateRandomNode("f2"))));
    BOOST_REQUIRE_EQUAL(size(),7);
    addNode(NodeSPtr(new Node(generateRandomNode("f3"))));
    BOOST_REQUIRE_EQUAL(size(),8);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),1);

    BOOST_REQUIRE_THROW(addNode(
                NodeSPtr()),std::invalid_argument);

    addNode(NodeSPtr(new Node(generateRandomNode("f4"))));
    BOOST_REQUIRE_EQUAL(size(),9);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
    addNode(NodeSPtr(new Node(generateRandomNode("f5"))));
    BOOST_REQUIRE_EQUAL(size(),10);
    addNode(NodeSPtr(new Node(generateRandomNode("f6"))));
    BOOST_REQUIRE_EQUAL(size(),11);
    addNode(NodeSPtr(new Node(generateRandomNode("04"))));
    BOOST_REQUIRE_EQUAL(size(),12);
    addNode(NodeSPtr(new Node(generateRandomNode("05"))));
    BOOST_REQUIRE_EQUAL(size(),13);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
}

BOOST_AUTO_TEST_CASE(addNode_oneSplit_toLower)
{
    addNode(NodeSPtr(new Node(generateRandomNode("00"))));
    BOOST_REQUIRE_EQUAL(size(),1);
    addNode(NodeSPtr(new Node(generateRandomNode("01"))));
    BOOST_REQUIRE_EQUAL(size(),2);
    addNode(NodeSPtr(new Node(generateRandomNode("02"))));
    BOOST_REQUIRE_EQUAL(size(),3);
    addNode(NodeSPtr(new Node(generateRandomNode("03"))));
    BOOST_REQUIRE_EQUAL(size(),4);
    addNode(NodeSPtr(new Node(generateRandomNode("f0"))));
    BOOST_REQUIRE_EQUAL(size(),5);
    addNode(NodeSPtr(new Node(generateRandomNode("f1"))));
    BOOST_REQUIRE_EQUAL(size(),6);
    addNode(NodeSPtr(new Node(generateRandomNode("f2"))));
    BOOST_REQUIRE_EQUAL(size(),7);
    addNode(NodeSPtr(new Node(generateRandomNode("f3"))));
    BOOST_REQUIRE_EQUAL(size(),8);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),1);

    addNode(NodeSPtr(new Node(generateRandomNode("04"))));
    BOOST_REQUIRE_EQUAL(size(),9);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
    addNode(NodeSPtr(new Node(generateRandomNode("05"))));
    BOOST_REQUIRE_EQUAL(size(),10);
    addNode(NodeSPtr(new Node(generateRandomNode("06"))));
    BOOST_REQUIRE_EQUAL(size(),11);
    addNode(NodeSPtr(new Node(generateRandomNode("f4"))));
    BOOST_REQUIRE_EQUAL(size(),12);
    addNode(NodeSPtr(new Node(generateRandomNode("f5"))));
    BOOST_REQUIRE_EQUAL(size(),13);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << a << b << c;
                addNode(NodeSPtr(new Node(generateRandomNode(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_999_to_000)
{
    int count = 0;
    for ( int a = 9; a >= 0; --a )
    {
        for ( int b = 9; b >= 0; --b )
        {
            for ( int c = 9; c >= 0; --c )
            {
                ++count;
                std::stringstream num;
                num << a << b << c;
                addNode(NodeSPtr(new Node(generateRandomNode(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_cba)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << c << b << a;
                addNode(NodeSPtr(new Node(generateRandomNode(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_bca)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << b << c << a;
                addNode(NodeSPtr(new Node(generateRandomNode(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_acb)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << a << c << b;
                addNode(NodeSPtr(new Node(generateRandomNode(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_cab)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << c << a << b;
                addNode(NodeSPtr(new Node(generateRandomNode(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addNode_inARow_000_to_999_bac)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << b << a << c;
                addNode(NodeSPtr(new Node(generateRandomNode(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addNode_someRandom)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<NodeSPtr> addresses;
        const size_t count = (rand()+1)%((rand()+1)%4096);
        for( size_t j = 0; j < count; j++ )
        {
            bool bad = false;
            NodeSPtr a(new Node(generateRandomNode()));
            BOOST_FOREACH( const NodeSPtr& it, addresses )
            {
                if (*a == *it)
                {
                    --j;
                    bad = true;
                    break;
                }
            }
            if (!bad)
            {
                addresses.push_back(a);
                addNode(a);
            }
        }
        this->clear();
        BOOST_REQUIRE_EQUAL(this->size(),0);
    }
}

BOOST_AUTO_TEST_CASE(addNode_and_find)
{
    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<NodeSPtr> v;
        v += NodeSPtr(new Node(generateRandomNode("00"))),
             NodeSPtr(new Node(generateRandomNode("01"))),
             NodeSPtr(new Node(generateRandomNode("02"))),
             NodeSPtr(new Node(generateRandomNode("03"))),
             NodeSPtr(new Node(generateRandomNode("f0"))),
             NodeSPtr(new Node(generateRandomNode("f1"))),
             NodeSPtr(new Node(generateRandomNode("f2"))),
             NodeSPtr(new Node(generateRandomNode("f3"))),
             NodeSPtr(new Node(generateRandomNode("f4"))),
             NodeSPtr(new Node(generateRandomNode("f5"))),
             NodeSPtr(new Node(generateRandomNode("f6"))),
             NodeSPtr(new Node(generateRandomNode("04"))),
             NodeSPtr(new Node(generateRandomNode("05")));

        BOOST_FOREACH( NodeSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(addNode(addr));
        }

        BOOST_FOREACH( const NodeSPtr& addr, v )
        {
            boost::optional<NodeSPtr> b = getNode(*addr);
            BOOST_CHECK(!!b);
            BOOST_REQUIRE(b.get());
            BOOST_CHECK_EQUAL(**b,*addr);
        }

        for ( int j = 0; j < TEST_LOOP_COUNT; ++j )
        {
            boost::optional<NodeSPtr> b = getNode(
                   NodeData::parse(generateRandomNode("b")));
            BOOST_CHECK(!b);
        }

        clear();
    }
}

BOOST_AUTO_TEST_CASE(removeNode_fail)
{
    NodeSPtr ptr;
    BOOST_REQUIRE_THROW(removeNode(ptr),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(removeNode_removeSerial)
{
    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<NodeSPtr> v;
        v += NodeSPtr(new Node(generateRandomNode("00"))),
             NodeSPtr(new Node(generateRandomNode("01"))),
             NodeSPtr(new Node(generateRandomNode("02"))),
             NodeSPtr(new Node(generateRandomNode("03"))),
             NodeSPtr(new Node(generateRandomNode("f0"))),
             NodeSPtr(new Node(generateRandomNode("f1"))),
             NodeSPtr(new Node(generateRandomNode("f2"))),
             NodeSPtr(new Node(generateRandomNode("f3"))),
             NodeSPtr(new Node(generateRandomNode("f4"))),
             NodeSPtr(new Node(generateRandomNode("f5"))),
             NodeSPtr(new Node(generateRandomNode("f6"))),
             NodeSPtr(new Node(generateRandomNode("04"))),
             NodeSPtr(new Node(generateRandomNode("05")));

        BOOST_FOREACH( NodeSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(addNode(addr));
        }

        BOOST_FOREACH( NodeSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(removeNode(addr));
        }

        BOOST_REQUIRE_EQUAL(size(),0);
    }
}

BOOST_AUTO_TEST_CASE(removeNode_removeRandomOrder)
{
    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<NodeSPtr> v;
        v += NodeSPtr(new Node(generateRandomNode("00"))),
             NodeSPtr(new Node(generateRandomNode("01"))),
             NodeSPtr(new Node(generateRandomNode("02"))),
             NodeSPtr(new Node(generateRandomNode("03"))),
             NodeSPtr(new Node(generateRandomNode("f0"))),
             NodeSPtr(new Node(generateRandomNode("f1"))),
             NodeSPtr(new Node(generateRandomNode("f2"))),
             NodeSPtr(new Node(generateRandomNode("f3"))),
             NodeSPtr(new Node(generateRandomNode("f4"))),
             NodeSPtr(new Node(generateRandomNode("f5"))),
             NodeSPtr(new Node(generateRandomNode("f6"))),
             NodeSPtr(new Node(generateRandomNode("04"))),
             NodeSPtr(new Node(generateRandomNode("05")));

        BOOST_FOREACH( NodeSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(addNode(addr));
        }

        while ( ! v.empty() )
        {
            const int idx = (rand()+1)%v.size();
            BOOST_REQUIRE_NO_THROW(removeNode(v[idx]));
            v.erase(v.begin()+idx);
        }

        BOOST_REQUIRE_EQUAL(size(),0);
    }
}

BOOST_AUTO_TEST_CASE(removeNode_addAndRemove)
{
    std::vector<NodeSPtr> v;

    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<NodeSPtr> v;
        v += NodeSPtr(new Node(generateRandomNode("00"))),
             NodeSPtr(new Node(generateRandomNode("01"))),
             NodeSPtr(new Node(generateRandomNode("02"))),
             NodeSPtr(new Node(generateRandomNode("03"))),
             NodeSPtr(new Node(generateRandomNode("f0"))),
             NodeSPtr(new Node(generateRandomNode("f1"))),
             NodeSPtr(new Node(generateRandomNode("f2"))),
             NodeSPtr(new Node(generateRandomNode("f3"))),
             NodeSPtr(new Node(generateRandomNode("f4"))),
             NodeSPtr(new Node(generateRandomNode("f5"))),
             NodeSPtr(new Node(generateRandomNode("f6")));

        BOOST_FOREACH( NodeSPtr& a, v)
        {
            BOOST_REQUIRE_NO_THROW(addNode(a));
        }
        BOOST_REQUIRE_EQUAL(size(),v.size());

        NodeSPtr a(new Node(generateRandomNode("04")));
        NodeSPtr b(new Node(generateRandomNode("05")));

        const int numberToRemove = 2;
        for( int j = 0; j < numberToRemove; ++j )
        {
            const int idx = (rand()+1)%v.size();
            BOOST_REQUIRE_NO_THROW(removeNode(v[idx]));
            v.erase(v.begin()+idx);
        }
        BOOST_REQUIRE_EQUAL(size(),v.size());
        BOOST_REQUIRE_NO_THROW(addNode(a));
        BOOST_REQUIRE_NO_THROW(addNode(b));
        BOOST_REQUIRE_EQUAL(size(),v.size()+numberToRemove);

        BOOST_FOREACH( NodeSPtr& a, v )
        {
            BOOST_REQUIRE_NO_THROW(removeNode(a));
        }
        BOOST_REQUIRE_EQUAL(size(),numberToRemove);
        BOOST_REQUIRE_NO_THROW(removeNode(a));
        BOOST_REQUIRE_NO_THROW(removeNode(b));
        BOOST_REQUIRE_EQUAL(size(),0);
    }
}

BOOST_AUTO_TEST_SUITE_END();
