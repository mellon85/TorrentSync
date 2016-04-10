#include <gtest/gtest.h>
#include <cstdlib>
#include <sstream>

#include <torrentsync/dht/RoutingTable.h>
#include <test/torrentsync/dht/CommonNodeTest.h>
#include <torrentsync/utils/log/Logger.h>

using namespace torrentsync::dht;
using namespace torrentsync::utils::log;
using boost::asio::ip::udp;


