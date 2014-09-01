#include <boost/test/unit_test.hpp>

#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/utils/Yield.h>

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
    
    auto buff = utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes26:HHHHHHHHHHHHHHHHHHHHEEEEGGe1:t2:aa1:y1:re");
    
    dht::NodeData source(utils::parseIDFromHex(id1));
    dht::NodeData target(utils::parseIDFromHex(id2));
    
    // 0x4747 => 'GG', 0x45454545 => 'EEEE'
    boost::optional<boost::asio::ip::udp::endpoint> endpoint(
        boost::asio::ip::udp::endpoint(
             boost::asio::ip::address_v4(0x45454545),0x4747));
    
    std::shared_ptr<dht::Node> match(new dht::Node(target.write(),endpoint));
    
    std::list<dht::NodeSPtr> nodes;
    nodes.push_back(match);

    utils::Buffer ret;
    BOOST_REQUIRE_NO_THROW(
        ret = reply::FindNode::make(
            transaction,
            source,
            utils::makeYield(nodes.cbegin(),nodes.cend()).function()));
            
    BOOST_REQUIRE(ret == buff);
    
    const auto m = dht::message::Message::parseMessage(ret);
    BOOST_REQUIRE(!!m);
    BOOST_REQUIRE(m->getType() == Type::Reply);
    BOOST_REQUIRE(m->getTransactionID() == transaction);
}

BOOST_AUTO_TEST_CASE(reply_multiple)
{
    const std::string id1("4747474747474747474747474747474747474747");
    const std::string id2("4848484848484848484848484848484848484848");
    const auto transaction = utils::makeBuffer("aa");
    
    auto buff = utils::makeBuffer("d1:rd2:id20:GGGGGGGGGGGGGGGGGGGG5:nodes78:HHHHHHHHHHHHHHHHHHHHEEEEGGHHHHHHHHHHHHHHHHHHHHEEEEGGHHHHHHHHHHHHHHHHHHHHEEEEGGe1:t2:aa1:y1:re");
    
    dht::NodeData source(utils::parseIDFromHex(id1));
    dht::NodeData target(utils::parseIDFromHex(id2));
    
    // 0x4747 => 'GG', 0x45454545 => 'EEEE'
    boost::optional<boost::asio::ip::udp::endpoint> endpoint(
        boost::asio::ip::udp::endpoint(
             boost::asio::ip::address_v4(0x45454545),0x4747));
    
    std::shared_ptr<dht::Node> match;
    
    BOOST_REQUIRE_NO_THROW(match.reset(new dht::Node(target.write(),endpoint)));
    
    std::list<dht::NodeSPtr> nodes;
    nodes.push_back(match);
    nodes.push_back(match);
    nodes.push_back(match);

    utils::Buffer ret;
    BOOST_REQUIRE_NO_THROW(
        ret = reply::FindNode::make(
            transaction,
            source,
            utils::makeYield(nodes.cbegin(),nodes.cend()).function()));
            
    BOOST_REQUIRE(ret == buff);
    
    const auto m = dht::message::Message::parseMessage(ret);
    BOOST_REQUIRE(!!m);
    BOOST_REQUIRE(m->getType() == Type::Reply);
    BOOST_REQUIRE(m->getTransactionID() == transaction);
}

BOOST_AUTO_TEST_SUITE_END();
