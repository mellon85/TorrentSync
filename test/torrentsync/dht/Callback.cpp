#include <boost/test/unit_test.hpp>
#include <sstream>

#include <torrentsync/dht/Callback.h>
#include <test/torrentsync/dht/CommonNodeTest.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_Callback);

using namespace torrentsync::dht;

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    const std::string type = "q";

    Callback call(Callback::callback(),
        type,
        Callback::filterPeer(),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
}

BOOST_AUTO_TEST_CASE(match)
{
    const std::string type = "q";

    Callback call(Callback::callback(),
        type,
        Callback::filterPeer(),
        Callback::filterTransactionID());

    BOOST_REQUIRE_EQUAL(false,call.isOld());
}

BOOST_AUTO_TEST_SUITE_END();

