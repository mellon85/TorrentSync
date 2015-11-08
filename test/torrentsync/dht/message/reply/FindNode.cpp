#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/dht/message/Messages.h>
#include <torrentsync/dht/Node.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>


BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_reply_FindNode);

using namespace torrentsync::dht::message;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(reply_perfectMatch)
{
    const std::string id1("4747474747474747474747474747474747474747");
    const std::string id2("4848484848484848484848484848484848484848");
    const auto transaction = utils::makeBuffer("aa");

    const auto buff = utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes26:HHHHHHHHHHHHHHHHHHHHEEFHGFe1:t2:aa1:y1:re");

    dht::NodeData source(utils::parseIDFromHex(id1));
    dht::NodeData target(utils::parseIDFromHex(id2));

    // 0x4747 => 'GG', 0x45454647 => 'EEDF'
    const auto endpoint(
        boost::asio::ip::udp::endpoint(
             boost::asio::ip::address_v4(0x45454648),0x4746));

    std::shared_ptr<dht::Node> match(new dht::Node(target.write(),endpoint));

    std::list<dht::NodeSPtr> nodes;
    nodes.push_back(match);

    utils::Buffer ret;
    BOOST_REQUIRE_NO_THROW(
        ret = reply::FindNode::make(
            transaction,
            source,
            nodes));

    BOOST_REQUIRE(ret == buff);

    const auto m = parseMessage(ret);
    BOOST_CHECK(getType(m) == Type::Reply);
    BOOST_CHECK(getTransactionID(m) == transaction);

    auto *r = boost::get<reply::Reply>(&m);
    BOOST_REQUIRE(r != nullptr);
    auto *find_node = boost::get<reply::FindNode>(r);
    std::vector<dht::NodeSPtr> peers;
    BOOST_REQUIRE_NO_THROW(peers = find_node->getNodes());
    BOOST_REQUIRE_EQUAL(peers.size(),nodes.size());
    BOOST_CHECK(find_node->getType() == Type::Reply);
    BOOST_CHECK(find_node->getTransactionID() == transaction);

    BOOST_REQUIRE(peers[0]->write() == utils::makeBuffer("HHHHHHHHHHHHHHHHHHHH"));
    BOOST_REQUIRE_EQUAL(peers[0]->getEndpoint()->address().to_v4().to_ulong(),
            endpoint.address().to_v4().to_ulong());
    BOOST_REQUIRE_EQUAL(peers[0]->getEndpoint()->port(), endpoint.port());
}

BOOST_AUTO_TEST_CASE(reply_multiple)
{
    const std::string id1("4747474747474747474747474747474747474747");
    const std::string id2("4848484848484848484848484848484848484848");
    const std::string id3("4141414141414141414141414141414141414141");
    const std::string id4("4242424242424242424242424242424242424242");
    const auto transaction = utils::makeBuffer("aa");

    auto buff = utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes78:HHHHHHHHHHHHHHHHHHHHEEGGFDAAAAAAAAAAAAAAAAAAAAEEGGFDBBBBBBBBBBBBBBBBBBBBEEGGFDe1:t2:aa1:y1:re");

    dht::NodeData source(utils::parseIDFromHex(id1));
    dht::NodeData target1(utils::parseIDFromHex(id2));
    dht::NodeData target2(utils::parseIDFromHex(id3));
    dht::NodeData target3(utils::parseIDFromHex(id4));

    boost::optional<boost::asio::ip::udp::endpoint> endpoint(
        boost::asio::ip::udp::endpoint(
             boost::asio::ip::address_v4(0x45454747),0x4644));

    std::list<dht::NodeSPtr> nodes;
    BOOST_REQUIRE_NO_THROW(
    nodes.push_back(dht::NodeSPtr(new dht::Node(target1.write(),endpoint)));
    nodes.push_back(dht::NodeSPtr(new dht::Node(target2.write(),endpoint)));
    nodes.push_back(dht::NodeSPtr(new dht::Node(target3.write(),endpoint))); );

    utils::Buffer ret;
    BOOST_REQUIRE_NO_THROW(
        ret = reply::FindNode::make(
            transaction,
            source,
            nodes));

    BOOST_REQUIRE(ret == buff);
    const auto m = parseMessage(ret);

    BOOST_CHECK(getType(m) == Type::Reply);
    BOOST_CHECK(getTransactionID(m) == transaction);

    auto *r = boost::get<reply::Reply>(&m);
    BOOST_REQUIRE(r != nullptr);
    auto *find_node = boost::get<reply::FindNode>(r);

    std::vector<dht::NodeSPtr> peers;
    BOOST_REQUIRE_NO_THROW(peers = find_node->getNodes());
    BOOST_REQUIRE_EQUAL(peers.size(),nodes.size());
    BOOST_CHECK(find_node->getType() == Type::Reply);
    BOOST_CHECK(find_node->getTransactionID() == transaction);

    BOOST_CHECK(peers[0]->write() == utils::makeBuffer("HHHHHHHHHHHHHHHHHHHH"));
    BOOST_CHECK_EQUAL(peers[0]->getEndpoint()->address().to_v4().to_ulong(), 0x45454747);
    BOOST_CHECK_EQUAL(peers[0]->getEndpoint()->port(), 0x4644);

    BOOST_CHECK(peers[1]->write() == utils::makeBuffer("AAAAAAAAAAAAAAAAAAAA"));
    BOOST_CHECK_EQUAL(peers[1]->getEndpoint()->address().to_v4().to_ulong(), 0x45454747);
    BOOST_CHECK_EQUAL(peers[1]->getEndpoint()->port(), 0x4644);

    BOOST_CHECK(peers[2]->write() == utils::makeBuffer("BBBBBBBBBBBBBBBBBBBB"));
    BOOST_CHECK_EQUAL(peers[2]->getEndpoint()->address().to_v4().to_ulong(), 0x45454747);
    BOOST_CHECK_EQUAL(peers[2]->getEndpoint()->port(), 0x4644);
}

BOOST_AUTO_TEST_SUITE_END();
