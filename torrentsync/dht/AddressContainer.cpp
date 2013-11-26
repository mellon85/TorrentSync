#include <torrentsync/dht/AddressContainer.h>

#include <exception>

namespace torrentsync
{
namespace dht
{

#ifndef HAS_Cxx11
namespace
{
template <class T>
inline size_t size_op( const size_t init,const T& t)
{
    return init+t->size();
}
};
#endif

AddressContainer::AddressContainer(
        const AddressData& nodeAddress) : nodeAddress(nodeAddress)
{
    // create first bucket
    boost::shared_ptr<Bucket> bucket(
            new Bucket( AddressData::minValue, AddressData::maxValue));
    buckets.insert(bucket);
}

AddressContainer::~AddressContainer()
{
}

void AddressContainer::addAddress( AddressSPtr address )
{
    if (!address.get())
        throw std::invalid_argument("Address is not set");

    UpgradableLock rlock(mutex);

    BucketContainer::const_iterator bucket_it = findBucket(*address);
    assert(bucket_it != buckets.end());

    BucketContainer::key_type bucket = *bucket_it;
    assert(bucket->inBounds(address));

    if ( bucket->canAcceptAddress(address) )
    {
        bucket->add(address);
    }
    else
    {
        UpgradedWriteLock wlock(rlock);
        std::pair<BucketSPtr,BucketSPtr> split_buckets = split(bucket_it);

        assert(split_buckets.first->inBounds(address) ||
               split_buckets.second->inBounds(address));

        if (split_buckets.first->inBounds(address))
        {
            assert(split_buckets.first->canAcceptAddress(address));
            split_buckets.first->add(address);
        }
        else
        {
            assert(split_buckets.second->inBounds(address));
            assert(split_buckets.second->canAcceptAddress(address));
            split_buckets.first->add(address);
        }
    }
}

void AddressContainer::removeAddress( AddressSPtr address )
{
    if (!address.get())
        throw std::invalid_argument("Address is not set");

    UpgradableLock rlock(mutex);

    BucketContainer::const_iterator bucket_it = findBucket(*address);
    assert(bucket_it != buckets.end());

    BucketContainer::key_type bucket = *bucket_it;
    assert(bucket->inBounds(address));

    bucket->remove(*address);

    if ( bucket->size() == 0 )
    {
        UpgradedWriteLock wlock(rlock);
        merge(bucket_it);
    }
}

size_t AddressContainer::size() const
{
    ReadLock rlock(mutex);
    return std::accumulate(buckets.begin(), buckets.end(), static_cast<size_t>(0),
#ifndef HAS_Cxx11
        size_op
#else
        [](const size_t init,const BucketContainer::key_type& t) -> size_t
            { return init+t->size(); }
#endif
            );
}

AddressContainer::BucketContainer::const_iterator
AddressContainer::findBucket( AddressData& addr ) const
{
    const BucketContainer::key_type key(new Bucket(addr,addr));
    const BucketContainer::const_iterator it = buckets.lower_bound(key);

    assert( it != buckets.end() );
    assert( it->get() );
    return it;
}

void AddressContainer::merge( BucketContainer::const_iterator bucket_it )
{
    assert(bucket_it != buckets.end());
    assert(bucket_it->get());

    if ( buckets.size() == 1 )
        return;

    // TODO
}

std::pair<AddressContainer::BucketSPtr,AddressContainer::BucketSPtr>
AddressContainer::split( BucketContainer::const_iterator bucket_it )
{
    assert(bucket_it != buckets.end());
    assert(bucket_it->get());
    BucketSPtr bucket = *bucket_it;

    if ( buckets.size() == 1 )
    {
        // split the space in 2 equal parts
        // split in 0x0 -> 0x8000::0000
        // 0x800::0001 -> 0xFFFF::FFFF
    }
    else // split as specifid by bep_0005
    {
    }

    // TODO
    return std::make_pair(bucket,bucket);
}

}; // dht
}; // torrentsync
