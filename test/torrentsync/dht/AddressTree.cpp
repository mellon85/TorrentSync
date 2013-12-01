
#include <boost/test/unit_test.hpp>
#include <boost/foreach.hpp>
#include <torrentsync/dht/AddressTree.h>
#include <test/torrentsync/dht/CommonAddressTest.h>

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

BOOST_AUTO_TEST_CASE(addAddress_someRandom)
{
    for ( int i = 0; i < TEST_LOOP_COUNT; ++i )
    {
        std::vector<AddressSPtr> addresses;
        const size_t count = rand()%(rand()%4096);
        for( int j = 0; j < count; j++ )
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
                BOOST_REQUIRE_EQUAL(this->size(),j+1);
            }
        }
        this->clear();
        BOOST_REQUIRE_EQUAL(this->size(),0);
    }
}

BOOST_AUTO_TEST_SUITE_END();
