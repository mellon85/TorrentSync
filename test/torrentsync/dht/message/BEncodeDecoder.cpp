#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>
#include <streambuf>
#include <turtle/mock.hpp>

#include <torrentsync/dht/message/BEncodeDecoder.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_BEncodeDecoder);

using namespace torrentsync::dht::message;
using namespace torrentsync;

#define TEST_FIELD(key,value) { \
    BOOST_REQUIRE(map.find(key) != map.end()); \
    BOOST_REQUIRE(map.find(key)->second == \
        utils::makeBuffer(value));}

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    BEncodeDecoder dec;
}

BOOST_AUTO_TEST_CASE(parse_emptyDictionary)
{
    std::istringstream str("de");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),0);
}

BOOST_AUTO_TEST_CASE(parse_oneElementDictionary)
{
    std::istringstream str("d1:a1:be");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),1);

    auto t = map.find("a")->second;
    auto b = utils::makeBuffer("b");
    TEST_FIELD("a","b");
}

BOOST_AUTO_TEST_CASE(parse_emptyList)
{
    std::istringstream str("le");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),0);
}

BOOST_AUTO_TEST_CASE(parse_dictionary)
{
    std::istringstream str("d1:a2:bb2:yy4:plple");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),2);
    TEST_FIELD("a","bb");
    TEST_FIELD("yy","plpl");
}

BOOST_AUTO_TEST_CASE(parse_list)
{
    std::istringstream str("l1:a2:bb2:yy4:plpli1ee");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),5);
    TEST_FIELD("0","a");
    TEST_FIELD("1","bb");
    TEST_FIELD("2","yy");
    TEST_FIELD("3","plpl");
    TEST_FIELD("4",1);
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithList)
{
    std::istringstream str("d1:a2:bb2:yy4:plpl1:ql1:a1:b1:cee");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),5);

    TEST_FIELD("a","bb");
    TEST_FIELD("yy","plpl");
    TEST_FIELD("q/0","a");
    TEST_FIELD("q/1","b");
    TEST_FIELD("q/2","c");
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithDictionary)
{
    std::istringstream str("d1:a2:bb2:yy4:plpl1:qd1:a1:b1:c4:ababee");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),4);

    TEST_FIELD("a","bb");
    TEST_FIELD("yy","plpl");
    TEST_FIELD("q/a","b");
    TEST_FIELD("q/c","abab");
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithDictionary_inTheMiddle)
{
    std::istringstream str("d1:a2:bb2:yy4:plpl1:qd1:a1:b1:c4:ababe1:r1:ce");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),5);

    TEST_FIELD("a","bb");
    TEST_FIELD("yy","plpl");
    TEST_FIELD("q/a","b");
    TEST_FIELD("q/c","abab");
    TEST_FIELD("r","c");
}

BOOST_AUTO_TEST_CASE(parse_listWithList)
{
    std::istringstream str("l1:a2:aa1:ql1:b2:ceee");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
    const DataMap& map = decoder.getData();
    BOOST_REQUIRE_EQUAL(map.size(),5);

    TEST_FIELD("0","a");
    TEST_FIELD("1","aa");
    TEST_FIELD("2","q");
    TEST_FIELD("3/0","b");
    TEST_FIELD("3/1","ce");
}
BOOST_AUTO_TEST_CASE(parse_error)
{
    std::istringstream str("aaaaa");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_THROW(decoder.parseMessage(str),BEncodeException);
}


BOOST_AUTO_TEST_CASE(parse_error2)
{
    std::istringstream str("d1:a2:be");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_THROW(decoder.parseMessage(str),BEncodeException);
}

BOOST_AUTO_TEST_CASE(parse_error3)
{
    std::istringstream str("d1:ab:be");

    BEncodeDecoder decoder;

    BOOST_REQUIRE_THROW(decoder.parseMessage(str),BEncodeException);
}

BOOST_AUTO_TEST_SUITE_END();
