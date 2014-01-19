#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>

#include <torrentsync/dht/message/BEncodeDecoder.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_BEncodeDecoder);

using namespace torrentsync::dht::message;

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    std::stringstream str;
    BEncoderDecoder enc(str);

}

// TODO setup mocking of class for the expected calls

BOOST_AUTO_TEST_SUITE_END();
