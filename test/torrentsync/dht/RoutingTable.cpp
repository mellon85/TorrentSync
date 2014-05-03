#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <sstream>

#include <turtle/mock.hpp>

#include <torrentsync/dht/RoutingTable.h>
#include <test/torrentsync/dht/CommonNodeTest.h>
#include <torrentsync/utils/log/Logger.h>

using namespace torrentsync::dht;
using namespace torrentsync::utils::log;
using boost::asio::ip::udp;

MOCK_BASE_CLASS(MockRoutingTable, torrentsync::dht::RoutingTable)
{
public:
    MockRoutingTable( const udp::endpoint e ) : RoutingTable(e) {}
    MockRoutingTable() : MockRoutingTable(udp::endpoint(udp::v4(),0)) {}

    MOCK_METHOD(initializeNetwork, 1);
};


BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_RoutingTable,MockRoutingTable);

BOOST_AUTO_TEST_CASE(constructor)
{
    MockRoutingTable table(udp::endpoint(udp::v4(),0));
}

BOOST_AUTO_TEST_CASE(initializing_addresses)
{
    _initial_addresses.push_back(
        torrentsync::dht::Peer(
            torrentsync::dht::Node(generateRandomNode()),
            udp::endpoint()));
    _initial_addresses.push_back(
        torrentsync::dht::Peer(
            torrentsync::dht::Node(generateRandomNode()),
            udp::endpoint()));
    _initial_addresses.push_back(
        torrentsync::dht::Peer(
            torrentsync::dht::Node(generateRandomNode()),
            udp::endpoint()));

    BOOST_REQUIRE_EQUAL( 3, _initial_addresses.size() );
    initializeTable();
    getIO_service().run();
    BOOST_REQUIRE_EQUAL( 0, _initial_addresses.size() );
}

BOOST_AUTO_TEST_SUITE_END();
