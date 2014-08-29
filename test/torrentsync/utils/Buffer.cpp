#include <boost/test/unit_test.hpp>
#include <iostream>

#include <torrentsync/utils/Buffer.h>

BOOST_AUTO_TEST_SUITE(torrentsync_utils_buffer);

using namespace torrentsync::utils;

BOOST_AUTO_TEST_CASE(initialize_and_destroy)
{
    Buffer buff1;
    Buffer buff2 = makeBuffer("abcdef");
    Buffer buff4 = makeBuffer("abcdef");
    buff4.resize(2);
    
    BOOST_REQUIRE(buff1.empty());
    BOOST_REQUIRE_EQUAL(buff1.size(),0);
    BOOST_REQUIRE(!buff2.empty());
    BOOST_REQUIRE_EQUAL(buff2.size(),6);
    BOOST_REQUIRE(!buff4.empty());
    BOOST_REQUIRE_EQUAL(buff4.size(),2);
}

BOOST_AUTO_TEST_CASE(no_diff_between_cstr_and_str)
{
    std::string str("aa");
    Buffer buff = makeBuffer("aa");

    BOOST_REQUIRE(buff == str);
}

BOOST_AUTO_TEST_CASE(delete_with_resize)
{
    Buffer buff = makeBuffer("abc");
    buff.resize(2);
    BOOST_REQUIRE(buff == "ab");
}

BOOST_AUTO_TEST_CASE(resize)
{
    Buffer buff = makeBuffer("abcdefg");

    buff.resize(2);
    BOOST_REQUIRE_EQUAL(memcmp("ab",buff.data(),2),0);
    BOOST_REQUIRE(!buff.empty());
    BOOST_REQUIRE_EQUAL(buff.size(),2);

    buff.resize(3);
    BOOST_REQUIRE_EQUAL(memcmp("ab",buff.data(),2),0);
    BOOST_REQUIRE(!buff.empty());
    BOOST_REQUIRE_EQUAL(buff.size(),3);

    buff[2] = 'f';
    BOOST_REQUIRE_EQUAL(memcmp("abf",buff.data(),3),0);
}

BOOST_AUTO_TEST_CASE(parseHex)
{
    Buffer buff;

    buff = parseIDFromHex("0000000000000000000000000000000000000000");

    std::for_each( buff.begin(), buff.end(), [&](uint8_t t ) {
        BOOST_REQUIRE_EQUAL(t,0);
    });
    
    buff = parseIDFromHex("0000000000000000000000000000000000000001");
        
    std::for_each( buff.begin(), buff.begin()+19, [&](uint8_t t ) {
        BOOST_REQUIRE_EQUAL(t,0);
    });
    BOOST_REQUIRE_EQUAL(buff[19],1);
    
    buff = parseIDFromHex("0000000000000000000000000000000000000100");   
    std::for_each( buff.begin(), buff.begin()+18, [&](uint8_t t ) {
        BOOST_REQUIRE_EQUAL(t,0);
    });
    BOOST_REQUIRE_EQUAL(buff[18],1);
    BOOST_REQUIRE_EQUAL(buff[19],0);
    
    buff = parseIDFromHex("0000000000000000000000000000000100000000");   
    BOOST_REQUIRE_EQUAL(buff[15],1);
    
    buff = parseIDFromHex("1000000000000000000000000000001F00000000");   
    BOOST_REQUIRE_EQUAL(buff[0],0x10);
    BOOST_REQUIRE_EQUAL(buff[15],0x1F);
}

BOOST_AUTO_TEST_CASE(toString_m)
{
    Buffer buff;

    buff = parseIDFromHex("4747474747474747474747474747474747474747");
    auto str = toString(buff);
       
    BOOST_REQUIRE_EQUAL(str,"GGGGGGGGGGGGGGGGGGGG");
}

BOOST_AUTO_TEST_SUITE_END();
