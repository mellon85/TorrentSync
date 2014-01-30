#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <cstdlib>
#include <sstream>
#include <streambuf>
#include <turtle/mock.hpp>

#include <torrentsync/dht/message/BEncodeDecoder.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_message_BEncodeDecoder);

using namespace torrentsync::dht::message;

MOCK_BASE_CLASS(MockDecoder, BEncodeDecoder)
{
public:
    // pure virtual methods can't use MOCK_METHOD
    MOCK_METHOD_EXT(onDictionaryStart, 0, void(), onDictionaryStart);
    MOCK_METHOD_EXT(onDictionaryEnd, 0, void(), onDictionaryEnd);
    MOCK_METHOD_EXT(onListStart, 0, void(), onListStart);
    MOCK_METHOD_EXT(onListEnd, 0, void(), onListEnd);
    MOCK_METHOD_EXT(onElement, 1, void(const std::string&), onElement_1);
    MOCK_METHOD_EXT(onElement, 2, void(const std::string&,const std::string&), onElement_2);
};

BOOST_AUTO_TEST_CASE(constructor_destructor)
{
    MockDecoder decoder;
}

BOOST_AUTO_TEST_CASE(parse_emptyDictionary)
{
    std::istringstream str("de");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onDictionaryStart ).in(s);
    MOCK_EXPECT( decoder.onDictionaryEnd   ).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_emptyList)
{
    std::istringstream str("le");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onListStart ).in(s);
    MOCK_EXPECT( decoder.onListEnd   ).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_dictionary)
{
    std::istringstream str("d1:a2:bb2:yy4:plple");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onDictionaryStart).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("a"), mock::equal("bb")).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("yy"), mock::equal("plpl")).in(s);
    MOCK_EXPECT( decoder.onDictionaryEnd ).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_list)
{
    std::istringstream str("l1:a2:bb2:yy4:plple");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onListStart ).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with(mock::equal("a")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with(mock::equal("bb")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with(mock::equal("yy")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with(mock::equal("plpl")).in(s);
    MOCK_EXPECT( decoder.onListEnd ).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithList)
{
    std::istringstream str("d1:a2:bb2:yy4:plpl1:ql1:a1:b1:cee");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onDictionaryStart ).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("a"), mock::equal("bb")).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("yy"), mock::equal("plpl")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("q")).in(s);
    MOCK_EXPECT( decoder.onListStart ).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("a")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("b")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("c")).in(s);
    MOCK_EXPECT( decoder.onListEnd ).in(s);
    MOCK_EXPECT( decoder.onDictionaryEnd ).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithDictionary)
{
    std::istringstream str("d1:a2:bb2:yy4:plpl1:qd1:a1:b1:c4:ababee");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onDictionaryStart ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("a"), mock::equal("bb")).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("yy"),mock::equal("plpl")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("q")).in(s);
    MOCK_EXPECT( decoder.onDictionaryStart ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("a"),mock::equal("b")).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("c"),mock::equal("abab")).in(s);
    MOCK_EXPECT( decoder.onDictionaryEnd ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onDictionaryEnd ).exactly(1).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_dictionaryWithDictionary_inTheMiddle)
{
    std::istringstream str("d1:a2:bb2:yy4:plpl1:qd1:a1:b1:c4:ababe1:r1:ce");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onDictionaryStart ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("a"), mock::equal("bb")).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("yy"),mock::equal("plpl")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("q")).in(s);
    MOCK_EXPECT( decoder.onDictionaryStart ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("a"),mock::equal("b")).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("c"),mock::equal("abab")).in(s);
    MOCK_EXPECT( decoder.onDictionaryEnd ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onElement_2 ).with( mock::equal("r"),mock::equal("c")).in(s);
    MOCK_EXPECT( decoder.onDictionaryEnd ).exactly(1).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}

BOOST_AUTO_TEST_CASE(parse_listWithList)
{
    std::istringstream str("l1:a2:aa1:ql1:b2:ceee");

    MockDecoder decoder;

    mock::sequence s;
    MOCK_EXPECT( decoder.onListStart ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("a")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("aa")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("q")).in(s);
    MOCK_EXPECT( decoder.onListStart ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("b")).in(s);
    MOCK_EXPECT( decoder.onElement_1 ).with( mock::equal("ce")).in(s);
    MOCK_EXPECT( decoder.onListEnd ).exactly(1).in(s);
    MOCK_EXPECT( decoder.onListEnd ).exactly(1).in(s);

    BOOST_REQUIRE_NO_THROW(decoder.parseMessage(str));
}


BOOST_AUTO_TEST_SUITE_END();
