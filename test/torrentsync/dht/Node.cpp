
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

#include <torrentsync/dht/Node.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

namespace
{
class NodeFixture : public torrentsync::dht::Node
{
public:
	NodeFixture() {}
};
};

BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_Node,NodeFixture);

using namespace torrentsync::dht;

BOOST_AUTO_TEST_CASE(distance_0)
{
    const std::string data = "ffffffffffffffff0000000000000001aaaaaaaa";

    Node *addr1;
    BOOST_REQUIRE_NO_THROW(addr1 = new Node(Node::parse(data)));
    Node *addr2;
    BOOST_REQUIRE_NO_THROW(addr2 = new Node(Node::parse(data)));

    BOOST_REQUIRE_EQUAL(data,addr1->string());
    BOOST_REQUIRE(boost::iequals(data,addr2->string()));

    const Distance dist = *addr1 ^ *addr2;
    BOOST_REQUIRE_EQUAL(dist.string(),"0000000000000000000000000000000000000000");

    delete addr1;
    delete addr2;
}

BOOST_AUTO_TEST_CASE(distance_some_static)
{
    const std::string data1 = "ffffffffffffffff0000000000000001aaaaaaaa";
    const std::string data2 = "ffffffffffffffff0001100000000001aaaaaaaa";

    Node *addr1;
    BOOST_REQUIRE_NO_THROW(addr1 = new Node(Node::parse(data1)));
    Node *addr2;
    BOOST_REQUIRE_NO_THROW(addr2 = new Node(Node::parse(data2)));

    BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
    BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

    const Distance dist = *addr1 ^ *addr2;
    BOOST_REQUIRE_EQUAL(dist.string(),"0000000000000000000110000000000000000000");
    BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
    BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

    delete addr1;
    delete addr2;
}

BOOST_AUTO_TEST_CASE(distance_random)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i)
    {
        std::string data1 = generateRandomNode();
        const std::string data2 = data1;

        for ( unsigned int numOfDiff = rand()%40+1;
                numOfDiff > 0; --numOfDiff)
        {
            const int n = rand() % 16;
            data1[rand()%data1.length()] = static_cast<char>(n < 10 ? n+48 : (n%6)+97);
        }

        Node *addr1;
        BOOST_REQUIRE_NO_THROW(addr1 = new Node(Node::parse(data1)));
        Node *addr2;
        BOOST_REQUIRE_NO_THROW(addr2 = new Node(Node::parse(data2)));

        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        const Distance dist = *addr1 ^ *addr2;
        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        Node *addr3;
        BOOST_REQUIRE_NO_THROW(addr3 = new Node(Node::parse(dist.string())));
        const Distance dist1 = *addr3 ^ *addr2;
        const Distance dist2 = *addr3 ^ *addr1;

        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        BOOST_REQUIRE_EQUAL(dist1.string(),addr1->string());
        BOOST_REQUIRE_EQUAL(dist2.string(),addr2->string());

        delete addr1;
        delete addr2;
        delete addr3;
    }
}

BOOST_AUTO_TEST_CASE(is_Good_Bad_Questionale)
{
    setGood();
    BOOST_REQUIRE_EQUAL(true,isGood());
    BOOST_REQUIRE_EQUAL(isBad(),false);
    BOOST_REQUIRE_EQUAL(isQuestionable(),false);

    _last_time_good = _last_time_good - good_interval;
    BOOST_REQUIRE_EQUAL(_last_unanswered_queries,0);

    BOOST_REQUIRE_EQUAL(isGood(),false);
    BOOST_REQUIRE_EQUAL(isQuestionable(),true);
    BOOST_REQUIRE_EQUAL(isBad(),false);

    _last_unanswered_queries = allowed_unanswered_queries+1;

    BOOST_REQUIRE_EQUAL(isGood(),false);
    BOOST_REQUIRE_EQUAL(isQuestionable(),false);
    BOOST_REQUIRE_EQUAL(isBad(),true);
}	

BOOST_AUTO_TEST_SUITE_END();

