
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

using namespace torrentsync;

BOOST_AUTO_TEST_CASE(distance_0)
{
    const std::string data = "ffffffffffffffff0000000000000001aaaaaaaa";
    auto dataBuff = utils::parseIDFromHex(data);

    Node *addr1;
    BOOST_REQUIRE_NO_THROW(addr1 = new Node(dataBuff));
    Node *addr2;
    BOOST_REQUIRE_NO_THROW(addr2 = new Node(dataBuff));

    BOOST_REQUIRE_EQUAL(data,addr1->string());
    BOOST_REQUIRE(boost::iequals(data,addr2->string()));

    const dht::Distance dist = *addr1 ^ *addr2;
    BOOST_REQUIRE_EQUAL(dist.string(),"0000000000000000000000000000000000000000");

    delete addr1;
    delete addr2;
}

BOOST_AUTO_TEST_CASE(distance_some_static)
{
    const std::string data1 = "ffffffffffffffff0000000000000001aaaaaaaa";
    auto dataBuff1 = utils::parseIDFromHex(data1);
    const std::string data2 = "ffffffffffffffff0001100000000001aaaaaaaa";
    auto dataBuff2 = utils::parseIDFromHex(data2);

    Node *addr1;
    BOOST_REQUIRE_NO_THROW(addr1 = new Node(dataBuff1));
    Node *addr2;
    BOOST_REQUIRE_NO_THROW(addr2 = new Node(dataBuff2));

    BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
    BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

    const dht::Distance dist = *addr1 ^ *addr2;
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

        auto dataBuff1 = utils::parseIDFromHex(data1);
        auto dataBuff2 = utils::parseIDFromHex(data2);

        Node *addr1;
        BOOST_REQUIRE_NO_THROW(addr1 = new Node(dataBuff1));
        Node *addr2;
        BOOST_REQUIRE_NO_THROW(addr2 = new Node(dataBuff2));

        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        const dht::Distance dist = *addr1 ^ *addr2;
        BOOST_REQUIRE(boost::iequals(data1,addr1->string()));
        BOOST_REQUIRE(boost::iequals(data2,addr2->string()));

        Node *addr3;
        BOOST_REQUIRE_NO_THROW(addr3 = new Node(utils::parseIDFromHex(dist.string())));
        const dht::Distance dist1 = *addr3 ^ *addr2;
        const dht::Distance dist2 = *addr3 ^ *addr1;

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

BOOST_AUTO_TEST_CASE(serialization)
{
    const utils::Buffer buff = utils::parseIDFromHex(generateRandomNode());
    dht::NodeData nodedata(buff);

    boost::asio::ip::udp::endpoint endpoint(
        boost::asio::ip::udp::endpoint(
             boost::asio::ip::address_v4(0x44454647),0x4445));

    dht::Node node1(nodedata.write(),endpoint);
    auto data = node1.getPackedNode();

    dht::Node node2(data.begin(), data.end());

    BOOST_CHECK_EQUAL(node2.getEndpoint()->port(), 0x4445);
    BOOST_CHECK_EQUAL(node2.getEndpoint()->address().to_v4().to_ulong(), 0x44454647);
    BOOST_CHECK(buff == node1.write());
    BOOST_CHECK(buff == node2.write());
    BOOST_CHECK(data == node2.getPackedNode());
}

BOOST_AUTO_TEST_SUITE_END();

