
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
#include <torrentsync/dht/AddressTree.h>
#include <test/torrentsync/dht/CommonAddressTest.h>
#include <boost/assign/std/vector.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/optional.hpp>

namespace
{
class AddressTreeFixture : public torrentsync::dht::AddressTree
{
public:
    AddressTreeFixture() : AddressTree(generateRandomAddress()) {}
};
};

BOOST_FIXTURE_TEST_SUITE(torrentsync_dht_AddressTree,AddressTreeFixture);

using namespace torrentsync::dht;
using namespace boost::assign;

BOOST_AUTO_TEST_CASE(addAddress_oneSplit_toUp)
{
    addAddress(AddressSPtr(new Address(generateRandomAddress("00"))));
    BOOST_REQUIRE_EQUAL(size(),1);
    addAddress(AddressSPtr(new Address(generateRandomAddress("01"))));
    BOOST_REQUIRE_EQUAL(size(),2);
    addAddress(AddressSPtr(new Address(generateRandomAddress("02"))));
    BOOST_REQUIRE_EQUAL(size(),3);
    addAddress(AddressSPtr(new Address(generateRandomAddress("03"))));
    BOOST_REQUIRE_EQUAL(size(),4);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f0"))));
    BOOST_REQUIRE_EQUAL(size(),5);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f1"))));
    BOOST_REQUIRE_EQUAL(size(),6);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f2"))));
    BOOST_REQUIRE_EQUAL(size(),7);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f3"))));
    BOOST_REQUIRE_EQUAL(size(),8);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),1);

    BOOST_REQUIRE_THROW(addAddress(
                AddressSPtr()),std::invalid_argument);

    addAddress(AddressSPtr(new Address(generateRandomAddress("f4"))));
    BOOST_REQUIRE_EQUAL(size(),9);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f5"))));
    BOOST_REQUIRE_EQUAL(size(),10);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f6"))));
    BOOST_REQUIRE_EQUAL(size(),11);
    addAddress(AddressSPtr(new Address(generateRandomAddress("04"))));
    BOOST_REQUIRE_EQUAL(size(),12);
    addAddress(AddressSPtr(new Address(generateRandomAddress("05"))));
    BOOST_REQUIRE_EQUAL(size(),13);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
}

BOOST_AUTO_TEST_CASE(addAddress_oneSplit_toLower)
{
    addAddress(AddressSPtr(new Address(generateRandomAddress("00"))));
    BOOST_REQUIRE_EQUAL(size(),1);
    addAddress(AddressSPtr(new Address(generateRandomAddress("01"))));
    BOOST_REQUIRE_EQUAL(size(),2);
    addAddress(AddressSPtr(new Address(generateRandomAddress("02"))));
    BOOST_REQUIRE_EQUAL(size(),3);
    addAddress(AddressSPtr(new Address(generateRandomAddress("03"))));
    BOOST_REQUIRE_EQUAL(size(),4);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f0"))));
    BOOST_REQUIRE_EQUAL(size(),5);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f1"))));
    BOOST_REQUIRE_EQUAL(size(),6);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f2"))));
    BOOST_REQUIRE_EQUAL(size(),7);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f3"))));
    BOOST_REQUIRE_EQUAL(size(),8);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),1);

    addAddress(AddressSPtr(new Address(generateRandomAddress("04"))));
    BOOST_REQUIRE_EQUAL(size(),9);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
    addAddress(AddressSPtr(new Address(generateRandomAddress("05"))));
    BOOST_REQUIRE_EQUAL(size(),10);
    addAddress(AddressSPtr(new Address(generateRandomAddress("06"))));
    BOOST_REQUIRE_EQUAL(size(),11);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f4"))));
    BOOST_REQUIRE_EQUAL(size(),12);
    addAddress(AddressSPtr(new Address(generateRandomAddress("f5"))));
    BOOST_REQUIRE_EQUAL(size(),13);
    BOOST_REQUIRE_EQUAL(getBucketsCount(),2);
}

BOOST_AUTO_TEST_CASE(addAddress_inARow_000_to_999)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << a << b << c;
                addAddress(AddressSPtr(new Address(generateRandomAddress(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addAddress_inARow_999_to_000)
{
    int count = 0;
    for ( int a = 9; a >= 0; --a )
    {
        for ( int b = 9; b >= 0; --b )
        {
            for ( int c = 9; c >= 0; --c )
            {
                ++count;
                std::stringstream num;
                num << a << b << c;
                addAddress(AddressSPtr(new Address(generateRandomAddress(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addAddress_inARow_000_to_999_cba)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << c << b << a;
                addAddress(AddressSPtr(new Address(generateRandomAddress(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addAddress_inARow_000_to_999_bca)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << b << c << a;
                addAddress(AddressSPtr(new Address(generateRandomAddress(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addAddress_inARow_000_to_999_acb)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << a << c << b;
                addAddress(AddressSPtr(new Address(generateRandomAddress(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addAddress_inARow_000_to_999_cab)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << c << a << b;
                addAddress(AddressSPtr(new Address(generateRandomAddress(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addAddress_inARow_000_to_999_bac)
{
    int count;
    for ( int a = 0; a < 9; ++a )
    {
        for ( int b = 0; b < 9; ++b )
        {
            for ( int c = 0; c < 9; ++c )
            {
                ++count;
                std::stringstream num;
                num << b << a << c;
                addAddress(AddressSPtr(new Address(generateRandomAddress(num.str()))));
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(addAddress_someRandom)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<AddressSPtr> addresses;
        const size_t count = (rand()+1)%((rand()+1)%4096);
        for( size_t j = 0; j < count; j++ )
        {
            bool bad = false;
            AddressSPtr a(new Address(generateRandomAddress()));
            BOOST_FOREACH( const AddressSPtr& it, addresses )
            {
                if (*a == *it)
                {
                    --j;
                    bad = true;
                    break;
                }
            }
            if (!bad)
            {
                addresses.push_back(a);
                addAddress(a);
            }
        }
        this->clear();
        BOOST_REQUIRE_EQUAL(this->size(),0);
    }
}

BOOST_AUTO_TEST_CASE(addAddress_and_find)
{
    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<AddressSPtr> v;
        v += AddressSPtr(new Address(generateRandomAddress("00"))),
             AddressSPtr(new Address(generateRandomAddress("01"))),
             AddressSPtr(new Address(generateRandomAddress("02"))),
             AddressSPtr(new Address(generateRandomAddress("03"))),
             AddressSPtr(new Address(generateRandomAddress("f0"))),
             AddressSPtr(new Address(generateRandomAddress("f1"))),
             AddressSPtr(new Address(generateRandomAddress("f2"))),
             AddressSPtr(new Address(generateRandomAddress("f3"))),
             AddressSPtr(new Address(generateRandomAddress("f4"))),
             AddressSPtr(new Address(generateRandomAddress("f5"))),
             AddressSPtr(new Address(generateRandomAddress("f6"))),
             AddressSPtr(new Address(generateRandomAddress("04"))),
             AddressSPtr(new Address(generateRandomAddress("05")));

        BOOST_FOREACH( AddressSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(addAddress(addr));
        }

        BOOST_FOREACH( const AddressSPtr& addr, v )
        {
            boost::optional<AddressSPtr> b = getAddress(*addr);
            BOOST_CHECK(!!b);
            BOOST_REQUIRE(b.get());
            BOOST_CHECK_EQUAL(**b,*addr);
        }

        for ( int j = 0; j < TEST_LOOP_COUNT; ++j )
        {
            boost::optional<AddressSPtr> b = getAddress(
                   AddressData(generateRandomAddress("b")));
            BOOST_CHECK(!b);
        }

        clear();
    }
}

BOOST_AUTO_TEST_CASE(removeAddress_fail)
{
    AddressSPtr ptr;
    BOOST_REQUIRE_THROW(removeAddress(ptr),std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(removeAddress_removeSerial)
{
    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<AddressSPtr> v;
        v += AddressSPtr(new Address(generateRandomAddress("00"))),
             AddressSPtr(new Address(generateRandomAddress("01"))),
             AddressSPtr(new Address(generateRandomAddress("02"))),
             AddressSPtr(new Address(generateRandomAddress("03"))),
             AddressSPtr(new Address(generateRandomAddress("f0"))),
             AddressSPtr(new Address(generateRandomAddress("f1"))),
             AddressSPtr(new Address(generateRandomAddress("f2"))),
             AddressSPtr(new Address(generateRandomAddress("f3"))),
             AddressSPtr(new Address(generateRandomAddress("f4"))),
             AddressSPtr(new Address(generateRandomAddress("f5"))),
             AddressSPtr(new Address(generateRandomAddress("f6"))),
             AddressSPtr(new Address(generateRandomAddress("04"))),
             AddressSPtr(new Address(generateRandomAddress("05")));

        BOOST_FOREACH( AddressSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(addAddress(addr));
        }

        BOOST_FOREACH( AddressSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(removeAddress(addr));
        }

        BOOST_REQUIRE_EQUAL(size(),0);
    }
}

BOOST_AUTO_TEST_CASE(removeAddress_removeRandomOrder)
{
    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<AddressSPtr> v;
        v += AddressSPtr(new Address(generateRandomAddress("00"))),
             AddressSPtr(new Address(generateRandomAddress("01"))),
             AddressSPtr(new Address(generateRandomAddress("02"))),
             AddressSPtr(new Address(generateRandomAddress("03"))),
             AddressSPtr(new Address(generateRandomAddress("f0"))),
             AddressSPtr(new Address(generateRandomAddress("f1"))),
             AddressSPtr(new Address(generateRandomAddress("f2"))),
             AddressSPtr(new Address(generateRandomAddress("f3"))),
             AddressSPtr(new Address(generateRandomAddress("f4"))),
             AddressSPtr(new Address(generateRandomAddress("f5"))),
             AddressSPtr(new Address(generateRandomAddress("f6"))),
             AddressSPtr(new Address(generateRandomAddress("04"))),
             AddressSPtr(new Address(generateRandomAddress("05")));

        BOOST_FOREACH( AddressSPtr addr, v )
        {
            BOOST_REQUIRE_NO_THROW(addAddress(addr));
        }

        while ( ! v.empty() )
        {
            const int idx = (rand()+1)%v.size();
            BOOST_REQUIRE_NO_THROW(removeAddress(v[idx]));
            v.erase(v.begin()+idx);
        }

        BOOST_REQUIRE_EQUAL(size(),0);
    }
}

BOOST_AUTO_TEST_CASE(removeAddress_addAndRemove)
{
    std::vector<AddressSPtr> v;

    for( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<AddressSPtr> v;
        v += AddressSPtr(new Address(generateRandomAddress("00"))),
             AddressSPtr(new Address(generateRandomAddress("01"))),
             AddressSPtr(new Address(generateRandomAddress("02"))),
             AddressSPtr(new Address(generateRandomAddress("03"))),
             AddressSPtr(new Address(generateRandomAddress("f0"))),
             AddressSPtr(new Address(generateRandomAddress("f1"))),
             AddressSPtr(new Address(generateRandomAddress("f2"))),
             AddressSPtr(new Address(generateRandomAddress("f3"))),
             AddressSPtr(new Address(generateRandomAddress("f4"))),
             AddressSPtr(new Address(generateRandomAddress("f5"))),
             AddressSPtr(new Address(generateRandomAddress("f6")));

        BOOST_FOREACH( AddressSPtr& a, v)
        {
            BOOST_REQUIRE_NO_THROW(addAddress(a));
        }
        BOOST_REQUIRE_EQUAL(size(),v.size());

        AddressSPtr a(new Address(generateRandomAddress("04")));
        AddressSPtr b(new Address(generateRandomAddress("05")));

        const int numberToRemove = 2;
        for( int j = 0; j < numberToRemove; ++j )
        {
            const int idx = (rand()+1)%v.size();
            BOOST_REQUIRE_NO_THROW(removeAddress(v[idx]));
            v.erase(v.begin()+idx);
        }
        BOOST_REQUIRE_EQUAL(size(),v.size());
        BOOST_REQUIRE_NO_THROW(addAddress(a));
        BOOST_REQUIRE_NO_THROW(addAddress(b));
        BOOST_REQUIRE_EQUAL(size(),v.size()+numberToRemove);

        BOOST_FOREACH( AddressSPtr& a, v )
        {
            BOOST_REQUIRE_NO_THROW(removeAddress(a));
        }
        BOOST_REQUIRE_EQUAL(size(),numberToRemove);
        BOOST_REQUIRE_NO_THROW(removeAddress(a));
        BOOST_REQUIRE_NO_THROW(removeAddress(b));
        BOOST_REQUIRE_EQUAL(size(),0);
    }
}

BOOST_AUTO_TEST_SUITE_END();
