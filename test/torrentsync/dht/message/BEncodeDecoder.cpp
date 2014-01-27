#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>
#include <streambuf>
#include <gmock/gmock.h>

#include <torrentsync/dht/message/BEncodeDecoder.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_BEncodeDecoder);

using namespace torrentsync::dht::message;

class MockDecoder : public BEncodeDecoder
{
public:
    MOCK_METHOD0(onDictionaryStart, void());
    MOCK_METHOD0(onDictionaryEnd, void());
    MOCK_METHOD0(onListStart, void());
    MOCK_METHOD0(onListEnd, void());
    MOCK_METHOD1(onElement, void(const std::string&));
    MOCK_METHOD2(onElement, void(const std::string&,const std::string&));
};

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    MockDecoder decoder();
}

BOOST_AUTO_TEST_CASE(parse_emptyDictionary)
{
    std::string message = "de";
    std::istringstream str;
    str.str(message);

    MockDecoder decoder;

    ::testing::Sequence s;
    EXPECT_CALL( decoder, onDictionaryStart() ).InSequence(s);
    EXPECT_CALL( decoder, onDictionaryEnd()   ).InSequence(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_emptyList)
{
    std::string message = "le";
    std::istringstream str;
    str.str(message);

    MockDecoder decoder;

    ::testing::Sequence s;
    EXPECT_CALL( decoder, onListStart() ).InSequence(s);
    EXPECT_CALL( decoder, onListEnd()   ).InSequence(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_dictionary)
{
    std::string message = "d1:a2:bb2:yy4:plple";
    std::istringstream str;
    str.str(message);

    MockDecoder decoder;

    ::testing::Sequence s;
    EXPECT_CALL( decoder, onDictionaryStart()).InSequence(s);
    EXPECT_CALL( decoder, onElement("a","bb")).InSequence(s);
    EXPECT_CALL( decoder, onElement("yy","plpl") ).InSequence(s);
    EXPECT_CALL( decoder, onDictionaryEnd()).InSequence(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_list)
{
    std::string message = "l1:a2:bb2:yy4:plple";
    std::istringstream str;
    str.str(message);

    MockDecoder decoder;

    ::testing::Sequence s;
    EXPECT_CALL( decoder, onListStart()).InSequence(s);
    EXPECT_CALL( decoder, onElement("a")).InSequence(s);
    EXPECT_CALL( decoder, onElement("bb")).InSequence(s);
    EXPECT_CALL( decoder, onElement("yy")).InSequence(s);
    EXPECT_CALL( decoder, onElement("plpl")).InSequence(s);
    EXPECT_CALL( decoder, onListEnd()).InSequence(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithList)
{
    std::string message = "d1:a2:bb2:yy4:plpl1:ql1:a1:b1:cee";
    std::istringstream str;
    str.str(message);

    MockDecoder decoder;

    ::testing::Sequence s;
    EXPECT_CALL( decoder, onDictionaryStart() ).InSequence(s);
    EXPECT_CALL( decoder, onElement("a","bb")).InSequence(s);
    EXPECT_CALL( decoder, onElement("yy","plpl")).InSequence(s);
    EXPECT_CALL( decoder, onElement("q")).InSequence(s);
    EXPECT_CALL( decoder, onListStart() ).InSequence(s);
    EXPECT_CALL( decoder, onElement("a")).InSequence(s);
    EXPECT_CALL( decoder, onElement("b")).InSequence(s);
    EXPECT_CALL( decoder, onElement("c")).InSequence(s);
    EXPECT_CALL( decoder, onListEnd() ).InSequence(s);
    EXPECT_CALL( decoder, onDictionaryEnd()).InSequence(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithDictionary)
{
    std::string message = "d1:a2:bb2:yy4:plpl1:qd1:a1:b1:c2:ababee";
    std::istringstream str;
    str.str(message);

    MockDecoder decoder;

    ::testing::Sequence s;
    EXPECT_CALL( decoder, onDictionaryStart() ).InSequence(s);
    EXPECT_CALL( decoder, onElement("a","bb")).InSequence(s);
    EXPECT_CALL( decoder, onElement("yy","plpl")).InSequence(s);
    EXPECT_CALL( decoder, onElement("q")).InSequence(s);
    EXPECT_CALL( decoder, onDictionaryStart() ).InSequence(s);
    EXPECT_CALL( decoder, onElement("a","b")).InSequence(s);
    EXPECT_CALL( decoder, onElement("c","abab")).InSequence(s);
    EXPECT_CALL( decoder, onDictionaryEnd()).InSequence(s);
    EXPECT_CALL( decoder, onDictionaryEnd()).InSequence(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

/*
BOOST_AUTO_TEST_CASE(parse_listWithList)
{
    std::string message = "l1:a2:aa1:ql1:b2:cee";
    std::istringstream str;
    str.str(message);

    MockDecoder decoder;

    ::testing::Sequence s;
    EXPECT_CALL( decoder, onListStart() ).InSequence(s);
    EXPECT_CALL( decoder, onElement("a")).InSequence(s);
    EXPECT_CALL( decoder, onElement("aa")).InSequence(s);
    EXPECT_CALL( decoder, onElement("q")).InSequence(s);
    EXPECT_CALL( decoder, onListStart() ).InSequence(s);
    EXPECT_CALL( decoder, onElement("b")).InSequence(s);
    EXPECT_CALL( decoder, onElement("c")).InSequence(s);
    EXPECT_CALL( decoder, onListEnd()).InSequence(s);
    EXPECT_CALL( decoder, onListEnd()).InSequence(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}
*/

BOOST_AUTO_TEST_SUITE_END();
