#include <boost/test/unit_test.hpp>
#include <iostream>

#include <torrentsync/utils/Buffer.h>

BOOST_AUTO_TEST_SUITE(torrentsync_utils_buffer);

using namespace torrentsync::utils;

BOOST_AUTO_TEST_CASE(initialize_and_destroy)
{
    Buffer buff1;
    Buffer buff2(10);
    Buffer buff3("abcdef");
    Buffer buff4("abcdef",2);

    BOOST_REQUIRE(buff1.empty());
    BOOST_REQUIRE_EQUAL(buff1.size(),0);
    BOOST_REQUIRE(!buff2.empty());
    BOOST_REQUIRE_EQUAL(buff2.size(),10);
    BOOST_REQUIRE(!buff3.empty());
    BOOST_REQUIRE_EQUAL(buff3.size(),6);
    BOOST_REQUIRE(!buff4.empty());
    BOOST_REQUIRE_EQUAL(buff4.size(),2);
}

BOOST_AUTO_TEST_CASE(resize)
{
    Buffer buff("abcdefg");

    buff.resize(2);
    BOOST_REQUIRE_EQUAL(memcmp("ab",buff.get(),2),0);
    BOOST_REQUIRE(!buff.empty());
    BOOST_REQUIRE_EQUAL(buff.size(),2);

    buff.resize(3);
    BOOST_REQUIRE_EQUAL(memcmp("ab",buff.get(),2),0);
    BOOST_REQUIRE(!buff.empty());
    BOOST_REQUIRE_EQUAL(buff.size(),3);

    buff.get()[2] = 'f';
    BOOST_REQUIRE_EQUAL(memcmp("abf",buff.get(),3),0);
}
BOOST_AUTO_TEST_SUITE_END();

