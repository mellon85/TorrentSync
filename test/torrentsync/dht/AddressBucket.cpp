
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/bind.hpp>
#include <cstdlib>
#include <sstream>

#include <torrentsync/dht/AddressBucket.h>
#include <test/torrentsync/dht/CommonAddressTest.h>

BOOST_AUTO_TEST_SUITE(torrentsync_dht_AddressBucket);

using namespace torrentsync::dht;

BOOST_AUTO_TEST_CASE(constructor)
{
    AddressData lowbound("0000000000000000000000000000000000000000");
    AddressData highbound("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    AddressBucket<8> bucket(lowbound,highbound);

    BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
    BOOST_REQUIRE(bucket.getUpperBound() == highbound);
    BOOST_REQUIRE_EQUAL(bucket.size(),0);
    BOOST_REQUIRE_EQUAL(bucket.maxSize(),8);

    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        const std::string str = generateRandomAddress();
        boost::shared_ptr<Address> addr(new Address(str));
        BOOST_REQUIRE_EQUAL(true,bucket.inBounds(addr));
    }

    BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
    BOOST_REQUIRE(bucket.getUpperBound() == highbound);
    BOOST_REQUIRE_EQUAL(bucket.size(),0);
    BOOST_REQUIRE_EQUAL(bucket.maxSize(),8);
}

BOOST_AUTO_TEST_CASE(outside)
{
    AddressData lowbound("0000000000000000000000000000000000000000");
    AddressData highbound("0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    AddressBucket<8> bucket(lowbound,highbound);

    BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
    BOOST_REQUIRE(bucket.getUpperBound() == highbound);
    BOOST_REQUIRE_EQUAL(bucket.size(),0);
    BOOST_REQUIRE_EQUAL(bucket.maxSize(),8);

    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::string str = generateRandomAddress("F");
        boost::shared_ptr<Address> addr(new Address(str));
        BOOST_REQUIRE_EQUAL(false,bucket.inBounds(addr));
    }

    BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
    BOOST_REQUIRE(bucket.getUpperBound() == highbound);
    BOOST_REQUIRE_EQUAL(bucket.size(),0);
    BOOST_REQUIRE_EQUAL(bucket.maxSize(),8);
}

BOOST_AUTO_TEST_CASE(inside)
{
    AddressData lowbound("0000000000000000000000000000000000000000");
    AddressData highbound("000002003400FFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    AddressBucket<2> bucket(lowbound,highbound);
    
    BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
    BOOST_REQUIRE(bucket.getUpperBound() == highbound);
    BOOST_REQUIRE_EQUAL(bucket.size(),0);
    BOOST_REQUIRE_EQUAL(bucket.maxSize(),2);

    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::string str = generateRandomAddress("000002003400");
        boost::shared_ptr<Address> addr(new Address(str));
        BOOST_REQUIRE_EQUAL(true,bucket.inBounds(addr));
    }

    BOOST_REQUIRE(bucket.getLowerBound() == lowbound);
    BOOST_REQUIRE(bucket.getUpperBound() == highbound);
    BOOST_REQUIRE_EQUAL(bucket.size(),0);
    BOOST_REQUIRE_EQUAL(bucket.maxSize(),2);
}

BOOST_AUTO_TEST_CASE(bucket_ordering)
{
    AddressData a1("0000000000000000000000000000000000000000");
    AddressData a2("000002003400FFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    AddressData a3("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");
    AddressData a2_1("0000020034010000000000000000000000000000");
    
    BOOST_REQUIRE(a1 <= a2);
    BOOST_REQUIRE(a2 <= a3);
    
    AddressBucket<3> b1(a1,a2);
    AddressBucket<3> b2(a2_1,a3);
    
    BOOST_REQUIRE(std::less<AddressBucket<3> >()(b1,b2));
}

BOOST_AUTO_TEST_CASE(add_remove)
{
    AddressData bot("0000000000000000000000000000000000000000");
    AddressData top("0FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        AddressBucket<10> bucket(bot,top);

        std::vector<boost::shared_ptr<Address> > addresses;

        for( int i = 0; i < 10; ++i)
        {
            boost::shared_ptr<Address> a = boost::shared_ptr<Address>(new Address(generateRandomAddress("0")));
            addresses.push_back(a);
            BOOST_REQUIRE_NO_THROW(BOOST_REQUIRE(bucket.add(a)));
            boost::shared_ptr<Address> f = boost::shared_ptr<Address>(new Address(generateRandomAddress("F")));
            BOOST_REQUIRE_THROW(bucket.add(f),std::invalid_argument);

            BOOST_FOREACH( const boost::shared_ptr<Address>& va, addresses)
            {
                BOOST_REQUIRE(std::find(bucket.cbegin(), bucket.cend(), va ) != bucket.cend());
            }
            BOOST_REQUIRE_EQUAL(bucket.size(),i+1);
        }
        boost::shared_ptr<Address> a = boost::shared_ptr<Address>(new Address(generateRandomAddress("0")));
        BOOST_REQUIRE_EQUAL(false,bucket.add(a));
        BOOST_REQUIRE_EQUAL(bucket.size(),10);

        BOOST_FOREACH( const boost::shared_ptr<Address>& va, addresses)
        {
            BOOST_REQUIRE(std::find( bucket.cbegin(), bucket.cend(), va ) != bucket.cend());
        }

        for( int i = addresses.size(); i > 0; --i )
        {
            BOOST_REQUIRE(addresses.size() > 0);
            const int index = rand()%addresses.size();
            const int start_size = addresses.size();
            boost::shared_ptr<Address> a = addresses[index];
            addresses.erase(addresses.begin()+index);
            BOOST_REQUIRE(a.get() > 0);


            BOOST_REQUIRE_NO_THROW(bucket.remove(*a));
            BOOST_REQUIRE_EQUAL( bucket.size(), start_size-1);

            BOOST_FOREACH( const boost::shared_ptr<Address>& va, addresses)
            {
                BOOST_REQUIRE(std::find(bucket.cbegin(), bucket.cend(), va ) != bucket.cend());
            }
            BOOST_REQUIRE(std::find(bucket.cbegin(), bucket.cend(), a) == bucket.cend());
        }

        BOOST_REQUIRE_EQUAL( bucket.size(), 0);
    }
}

class FakeAddress : public torrentsync::dht::Address
{
public:
    FakeAddress( const std::string& str ) : Address(str) {}
    
    time_t& getTime() { return Address::last_time_good; }
    size_t& getLastUnansweredQueries() { return Address::last_unanswered_queries; }
};

BOOST_AUTO_TEST_CASE(removeBad)
{
    AddressData bot("0000000000000000000000000000000000000000");
    AddressData top("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<boost::shared_ptr<Address> > addresses;

        for( int i = 0; i < 10; ++i)
        {
            boost::shared_ptr<Address> a = boost::shared_ptr<Address>(new FakeAddress(generateRandomAddress()));
            addresses.push_back(a);
        }

        for( int j = 0; j < TEST_LOOP_COUNT; ++j )
        {
            AddressBucket<10> bucket(bot,top);
            BOOST_FOREACH( boost::shared_ptr<Address>& a, addresses)
            {   // deep copy into bucket
                boost::shared_ptr<Address> new_a(new Address(*a));
                BOOST_REQUIRE_NO_THROW(BOOST_REQUIRE(bucket.add(new_a)));
            }

            const int setbad_count = rand() % bucket.size();
            const int size = bucket.size();

            int setbad = 0;
            for( int bad = 0; bad < setbad_count; ++bad)
            {
                boost::shared_ptr<Address> a = *(bucket.cbegin()+rand()%bucket.size());
                FakeAddress* af = reinterpret_cast<FakeAddress*>(a.get());
                if (af->getTime() > 0)
                    ++setbad;
                af->getTime() = 0;
                af->getLastUnansweredQueries() = torrentsync::dht::Address::allowed_unanswered_queries+1;
                BOOST_REQUIRE(af->isBad());
            }
            BOOST_REQUIRE_NO_THROW( bucket.removeBad() );
            BOOST_REQUIRE_EQUAL( bucket.size(), size-setbad );
        }
    }
}

BOOST_AUTO_TEST_CASE(removeBad_2)
{
    AddressData bot("0000000000000000000000000000000000000000");
    AddressData top("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<boost::shared_ptr<Address> > addresses;

        for( int i = 0; i < 10; ++i)
        {
            boost::shared_ptr<Address> a = boost::shared_ptr<Address>(new FakeAddress(generateRandomAddress()));
            addresses.push_back(a);
        }

        AddressBucket<10> bucket(bot,top);
        while (bucket.size() > 0)
        {
            BOOST_FOREACH( boost::shared_ptr<Address>& a, addresses)
            {   // deep copy into bucket
                boost::shared_ptr<Address> new_a(new Address(*a));
                BOOST_REQUIRE_NO_THROW(BOOST_REQUIRE(bucket.add(new_a)));
            }

            const int setbad_count = rand() % bucket.size();
            const int size = bucket.size();

            int setbad = 0;
            for( int bad = 0; bad < setbad_count; ++bad)
            {
                boost::shared_ptr<Address> a = *(bucket.cbegin()+rand()%bucket.size());
                FakeAddress* af = reinterpret_cast<FakeAddress*>(a.get());
                if (af->getTime() > 0)
                    ++setbad;
                af->getTime() = 0;
                af->getLastUnansweredQueries() = torrentsync::dht::Address::allowed_unanswered_queries+1;
                BOOST_REQUIRE(af->isBad());
            }
            BOOST_REQUIRE_NO_THROW( bucket.removeBad() );
            BOOST_REQUIRE_EQUAL( bucket.size(), size-setbad );
        }
        BOOST_REQUIRE_EQUAL( bucket.size(), 0 );
    }
}

BOOST_AUTO_TEST_CASE(addIsSorted)
{
    AddressData bot("0000000000000000000000000000000000000000");
    AddressData top("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    std::vector<Address> addresses = {
    Address("0000000000000000000000000000000000000001"),
    Address("0000000000000000000000000000000000000002"),
    Address("00000000000000000000000000F0000000000000"),
    Address("00000000000000000000F0000000000000000000"),
    Address("0000000000000000F00000000000000000000000")};

    std::vector<Address> sorted_addresses = addresses;

    do
    {
        AddressBucket<10> bucket(bot,top);
        BOOST_FOREACH(const Address& addr, addresses)
        {
            boost::shared_ptr<Address> a(new Address(addr));
            bucket.add(a);
        }
        std::vector<Address>::iterator it2 = sorted_addresses.begin();
        for ( AddressBucket<10>::const_iterator it = bucket.cbegin();
              it != bucket.cend(); ++it, ++it2)
        {
            BOOST_REQUIRE_EQUAL(**it,*it2);
        }
    } while(std::next_permutation(addresses.begin(),addresses.end()));
}

BOOST_AUTO_TEST_CASE(addIsSortedRandom)
{
    AddressData bot("0000000000000000000000000000000000000000");
    AddressData top("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF");

    for( int loop = 0; loop < TEST_LOOP_COUNT; ++loop )
    {
        std::vector<Address> addresses;
        for( int i = 0; i < 10; ++i)
        {
            addresses.push_back(Address(generateRandomAddress()));
        }
        std::vector<Address> sorted_addresses = addresses;
        std::sort(sorted_addresses.begin(),sorted_addresses.end());

        AddressBucket<10> bucket(bot,top);
        BOOST_FOREACH(const Address& addr, addresses)
        {
            boost::shared_ptr<Address> a(new Address(addr));
            bucket.add(a);
        }
        std::vector<Address>::iterator it2 = sorted_addresses.begin();
        for ( AddressBucket<10>::const_iterator it = bucket.cbegin();
              it != bucket.cend(); ++it, ++it2)
        {
            BOOST_REQUIRE_EQUAL(**it,*it2);
        }
    }
}
BOOST_AUTO_TEST_SUITE_END();
