#include <boost/test/unit_test.hpp>
#include <boost/lambda/lambda.hpp>
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
    BOOST_REQUIRE_EQUAL(buff2.get()[buff2.size()],0);
    BOOST_REQUIRE(!buff3.empty());
    BOOST_REQUIRE_EQUAL(buff3.size(),6);
    BOOST_REQUIRE_EQUAL(buff3.get()[buff3.size()],0);
    BOOST_REQUIRE(!buff4.empty());
    BOOST_REQUIRE_EQUAL(buff4.size(),2);
    BOOST_REQUIRE_EQUAL(buff4.get()[buff4.size()],0);
}

BOOST_AUTO_TEST_CASE(no_diff_between_cstr_and_str)
{
    std::string str("aa");
    Buffer buff("aa");

    BOOST_REQUIRE_EQUAL(buff,str.c_str());
    BOOST_REQUIRE_EQUAL(buff,str);
}

BOOST_AUTO_TEST_CASE(delete_with_resize)
{
    Buffer buff("abc");
    buff.resize(2);
    BOOST_REQUIRE_EQUAL(buff,"ab");
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

BOOST_AUTO_TEST_CASE(freeze)
{
    Buffer x("abdef");
    Buffer y(x);

    BOOST_REQUIRE_EQUAL(y,x);
    BOOST_REQUIRE_EQUAL(y.begin(),x.begin()); // same pointers

    x.freeze();
    *x.begin() = *x.begin();

    BOOST_REQUIRE_EQUAL(y,x);
    BOOST_CHECK_PREDICATE( boost::lambda::_1 != boost::lambda::_2, (x.begin())(y.begin()));
}

BOOST_AUTO_TEST_CASE(constructor_equality)
{
    std::string content = "abcdefghi";

    Buffer a(content);
    Buffer b(content.c_str());
    Buffer c(content.c_str(),content.size());
    BOOST_REQUIRE_EQUAL(a,b);
    BOOST_REQUIRE_EQUAL(b,c);
}

BOOST_AUTO_TEST_CASE(test_zero_size)
{
    // to avoid confusin the compiler with 0 as a pointer I have to cast
    // it.
    Buffer buff(static_cast<size_t>(0));

    BOOST_REQUIRE_NO_THROW(
        buff.begin());
    BOOST_REQUIRE_NO_THROW(
        buff.end());
    BOOST_REQUIRE_NO_THROW(
        buff.cbegin());
    BOOST_REQUIRE_NO_THROW(
        buff.cend());

    BOOST_REQUIRE_NO_THROW(
        BOOST_REQUIRE_EQUAL(buff.begin(),buff.end()));
}

BOOST_AUTO_TEST_SUITE_END();
