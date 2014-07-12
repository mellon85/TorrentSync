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

boost::asio::io_service _service;
boost::asio::ip::udp::endpoint ep = udp::endpoint(udp::v4(),0);

MOCK_BASE_CLASS(MockRoutingTable, torrentsync::dht::RoutingTable)
{
public:

    MockRoutingTable() : RoutingTable(_service) {}

    MOCK_METHOD_EXT(sendMessage, 2, void (
        const torrentsync::utils::Buffer&,
        const udp::endpoint& addr), sendMessage);
};


BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_RoutingTable,MockRoutingTable);


BOOST_AUTO_TEST_CASE(initializing_addresses)
{
    _initial_addresses.push_back(boost::asio::ip::udp::endpoint());
    _initial_addresses.push_back(boost::asio::ip::udp::endpoint());
    _initial_addresses.push_back(boost::asio::ip::udp::endpoint());

    MOCK_EXPECT(sendMessage).exactly(3);
    BOOST_REQUIRE_EQUAL( 3, _initial_addresses.size() );
    initializeTable();
    _service.run_one();
    BOOST_REQUIRE_EQUAL( 0, _initial_addresses.size() );
}

BOOST_AUTO_TEST_CASE(initializing_addresses2)
{
    for ( size_t i = 0; i < INITIALIZE_PING_BATCH_SIZE; ++i)
        _initial_addresses.push_back(boost::asio::ip::udp::endpoint());
    _initial_addresses.push_back(boost::asio::ip::udp::endpoint());

    MOCK_EXPECT(sendMessage).exactly(INITIALIZE_PING_BATCH_SIZE);
    BOOST_REQUIRE_EQUAL( INITIALIZE_PING_BATCH_SIZE+1, _initial_addresses.size() );
    initializeTable();
    _service.run_one();
    BOOST_REQUIRE_EQUAL( 1, _initial_addresses.size() );
}

BOOST_AUTO_TEST_SUITE_END();
