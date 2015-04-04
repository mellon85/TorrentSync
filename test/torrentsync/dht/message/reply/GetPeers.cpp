#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/reply/GetPeers.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/utils/Yield.h>

#include <test/torrentsync/dht/CommonNodeTest.h>

#include <sstream>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_reply_GetPeers);

using namespace torrentsync::dht::message;
using namespace torrentsync;

BOOST_AUTO_TEST_CASE(reply_nodes)
{
    const std::string id1("4747474747474747474747474747474747474747");
    const std::string id2("4848484848484848484848484848484848484848");
    const std::string token("46464646");
    const auto transaction = utils::makeBuffer("aa");

    auto buff = utils::makeBuffer(
        "d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes52:HHHHHHHHHHHHHHHHHHHHEEGGFDHHHHHHHHHHHHHHHHHHHHEEGGFD5:token8:46464646e1:t2:aa1:y1:re");

    dht::NodeData source(utils::parseIDFromHex(id1));
    dht::NodeData target(utils::parseIDFromHex(id2));

    // 0x4747 => 'GG', 0x45454545 => 'EEEE'
    boost::optional<boost::asio::ip::udp::endpoint> endpoint(
        boost::asio::ip::udp::endpoint(
             boost::asio::ip::address_v4(0x45454747),0x4644));

    std::shared_ptr<dht::Node> match(new dht::Node(target.write(),endpoint));

    std::list<dht::NodeSPtr> nodes;
    nodes.push_back(match);
    nodes.push_back(match);

    utils::Buffer ret;
    BOOST_REQUIRE_NO_THROW(
        ret = reply::GetPeers::make(
            transaction,
            utils::makeBuffer(token),
            source,
            utils::makeYield(nodes.cbegin(),nodes.cend()).function()));

    BOOST_REQUIRE(ret == buff);

    const auto m = dht::message::Message::parseMessage(ret);
    BOOST_REQUIRE(!!m);
    BOOST_CHECK(m->getType() == Type::Reply);
    BOOST_CHECK(m->getTransactionID() == transaction);

    std::vector<dht::NodeSPtr> peers;
    const reply::GetPeers get_peers(*m);
    BOOST_REQUIRE_NO_THROW(
        BOOST_REQUIRE(!!get_peers.getNodes());
        BOOST_REQUIRE(!get_peers.getPeers()));
    BOOST_REQUIRE_NO_THROW(peers = *get_peers.getNodes());
    BOOST_REQUIRE_EQUAL(peers.size(),nodes.size());
    BOOST_CHECK(get_peers.getType() == Type::Reply);
    BOOST_CHECK(get_peers.getTransactionID() == transaction);

    BOOST_REQUIRE(peers[0]->write() == utils::makeBuffer("HHHHHHHHHHHHHHHHHHHH")); BOOST_REQUIRE_EQUAL(peers[0]->getEndpoint()->address().to_v4().to_ulong(), 0x45454747);
    BOOST_REQUIRE_EQUAL(peers[0]->getEndpoint()->port(), 0x4644);
}

BOOST_AUTO_TEST_CASE(reply_nodes_failed)
{
    const std::string id1("4747474747474747474747474747474747474747");
    const std::string id2("4848484848484848484848484848484848484848");
    const std::string token("46464646");
    const auto transaction = utils::makeBuffer("aa");

    dht::NodeData source(utils::parseIDFromHex(id1));
    dht::NodeData target(utils::parseIDFromHex(id2));

    std::list<dht::NodeSPtr> nodes;

    utils::Buffer ret;
    BOOST_REQUIRE_THROW(
        ret = reply::GetPeers::make(
            transaction,
            utils::makeBuffer(token),
            source,
            utils::makeYield(nodes.cbegin(),nodes.cend()).function()),
        dht::message::MessageException);
}

BOOST_AUTO_TEST_CASE(reply_peers)
{
    const std::string id1("4747474747474747474747474747474747474747");
    const std::string token("46464646");
    const auto transaction = utils::makeBuffer("aa");

    auto buff = utils::makeBuffer(
        "d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:token8:464646466:valuesl6:EEGGFD6:EEGGFDee1:t2:aa1:y1:re");

    dht::NodeData source(utils::parseIDFromHex(id1));

    // 0x4747 => 'GG', 0x45454545 => 'EEEE'
    boost::asio::ip::udp::endpoint endpoint(
        boost::asio::ip::address_v4(0x45454747),0x4644);

    std::list<boost::asio::ip::udp::endpoint> endpoints;
    endpoints.push_back(endpoint);
    endpoints.push_back(endpoint);

    utils::Buffer ret;
    BOOST_REQUIRE_NO_THROW(
        ret = reply::GetPeers::make(
            transaction,
            utils::makeBuffer(token),
            source,
            utils::makeYield(endpoints.cbegin(),endpoints.cend()).function()));

    BOOST_REQUIRE(ret == buff);

    const auto m = dht::message::Message::parseMessage(ret);
    BOOST_REQUIRE(!!m);
    BOOST_CHECK(m->getType() == Type::Reply);
    BOOST_CHECK(m->getTransactionID() == transaction);

    std::list<boost::asio::ip::udp::endpoint> peers;

    reply::GetPeers get_peers(*m);

    BOOST_CHECK(get_peers.getType() == Type::Reply);
    BOOST_CHECK(get_peers.getTransactionID() == transaction);
    BOOST_REQUIRE_NO_THROW(
        BOOST_REQUIRE(!get_peers.getNodes());
        BOOST_REQUIRE(!!get_peers.getPeers()));
    BOOST_REQUIRE_NO_THROW(peers = *get_peers.getPeers());

    BOOST_REQUIRE_EQUAL(peers.size(),endpoints.size());
    BOOST_REQUIRE(*peers.begin() == *endpoints.begin());
    BOOST_REQUIRE(*(++peers.begin()) == *(++endpoints.begin()));
}

BOOST_AUTO_TEST_CASE(reply_peers_failed)
{
    const std::string id1("4747474747474747474747474747474747474747");
    const std::string token("46464646");
    const auto transaction = utils::makeBuffer("aa");

    dht::NodeData source(utils::parseIDFromHex(id1));

    std::list<boost::asio::ip::udp::endpoint> nodes;

    utils::Buffer ret;
    BOOST_REQUIRE_THROW(
        ret = reply::GetPeers::make(
            transaction,
            utils::makeBuffer(token),
            source,
            utils::makeYield(nodes.cbegin(),nodes.cend()).function()),
        dht::message::MessageException);
}

BOOST_AUTO_TEST_SUITE_END();
