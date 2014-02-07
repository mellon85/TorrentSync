#include <torrentsync/dht/AddressTree.h>

#include <exception>
#include <numeric>

namespace torrentsync
{
namespace dht
{

#ifndef HAS_Cxx11
namespace
{
template <class T>
size_t size_op( const size_t init,const T& t)
{
    return init+t->size();
}
};
#endif

AddressTree::AddressTree(
        const AddressData nodeAddress) : nodeAddress(nodeAddress)
{
    clear();
}

AddressTree::~AddressTree()
{
}

bool AddressTree::addAddress( AddressSPtr address )
{
    if (!address.get())
        throw std::invalid_argument("Address is not set");

    UpgradableLock rlock(mutex);

    BucketContainer::const_iterator bucket_it = findBucket(*address);
    assert(bucket_it != buckets.end());

    BucketContainer::key_type bucket = *bucket_it;
    assert(bucket->inBounds(address));

    const bool isAdded = bucket->add(address);
    if ( !isAdded && bucket->inBounds(nodeAddress) )
    {
        UpgradedWriteLock wlock(rlock);
        MaybeBuckets maybe_split_buckets = split(bucket_it);

        // unsplittable, ignore it
        if (!maybe_split_buckets)
            return isAdded;

        BucketSPtrPair& split_buckets = *maybe_split_buckets;

        assert(split_buckets.first->inBounds(address) ||
               split_buckets.second->inBounds(address));

        if (split_buckets.first->inBounds(address))
        {
            split_buckets.first->add(address);
        }
        else
        {
            assert(split_buckets.second->inBounds(address));
            split_buckets.second->add(address);
        }
    }

    return isAdded;
}

void AddressTree::removeAddress( AddressSPtr address )
{
    if (!address.get())
        throw std::invalid_argument("Address is not set");

    UpgradableLock rlock(mutex);

    BucketContainer::const_iterator bucket_it = findBucket(*address);
    assert(bucket_it != buckets.end());

    BucketContainer::key_type bucket = *bucket_it;
    assert(bucket->inBounds(address));

    bucket->remove(*address);
}

size_t AddressTree::size() const
{
    ReadLock rlock(mutex);
#ifndef HAS_Cxx11
    size_t ret = 0;
    for ( BucketContainer::iterator it = buckets.begin();
            it != buckets.end(); ++it)
    {
        ret += (*it)->size();
    }
    return ret;
#else
    return std::accumulate(buckets.begin(), buckets.end(), static_cast<size_t>(0),
        [](const size_t init,const BucketContainer::key_type& t) -> size_t
            { return init+t->size(); });
#endif
}

BucketContainer::const_iterator
AddressTree::findBucket( const AddressData& addr ) const
{
    const BucketContainer::key_type key(new Bucket(addr,addr));

    
    BucketContainer::const_iterator it = buckets.begin();
    while ( it != buckets.end() && ! (*it)->inBounds(addr))
    {
        ++it;
    }

    /*
    BucketContainer::const_iterator it = buckets.lower_bound(key);

    if (it == buckets.end() )
        it = buckets.begin();
        */

    assert( it != buckets.end() );
    assert( it->get() );
    return it;
}

MaybeBuckets
AddressTree::split( BucketContainer::const_iterator bucket_it )
{
    assert(bucket_it != buckets.end());
    assert(bucket_it->get());
    BucketSPtr bucket = *bucket_it;

    MaybeBounds bounds = AddressData::splitInHalf(
            bucket->getLowerBound(), bucket->getUpperBound());

    if (!bounds)
        return MaybeBuckets();

    BucketSPtr lower_bucket(new Bucket(bucket->getLowerBound(),bounds->first));
    BucketSPtr upper_bucket(new Bucket(bounds->second,bucket->getUpperBound()));
    
    // split addresses in buckets
    for( Bucket::const_iterator it = bucket->cbegin(); it != bucket->cend(); ++it)
    {
        if (lower_bucket->inBounds(*it))
        {
            lower_bucket->add(*it);
        }
        else
        {
            assert(upper_bucket->inBounds(*it));
            upper_bucket->add(*it);
        }
    }

    assert(upper_bucket->size() + lower_bucket->size() == bucket->size());
 
    buckets.insert(bucket_it,upper_bucket);
    buckets.insert(bucket_it,lower_bucket);
    buckets.erase(bucket_it);

    return MaybeBuckets(BucketSPtrPair(lower_bucket,upper_bucket));
}

void AddressTree::clear()
{
    WriteLock lock(mutex);
    buckets.clear();

    // initialize first bucket
    boost::shared_ptr<Bucket> bucket(
            new Bucket( AddressData::minValue, AddressData::maxValue));
    buckets.insert(bucket);
}

const boost::optional<AddressSPtr> AddressTree::getAddress(
    const AddressData& data ) const
{
    return (*findBucket(data))->find(data);
}

}; // dht
}; // torrentsync
