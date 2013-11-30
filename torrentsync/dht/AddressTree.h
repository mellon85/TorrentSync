#pragma once
 
#include <torrentsync/dht/Address.h>
#include <torrentsync/dht/AddressBucket.h>
#include <torrentsync/dht/DHTConstants.h>
 
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/thread/shared_mutex.hpp>
 
#include <set>
 
namespace torrentsync
{
namespace dht
{
typedef AddressBucket<DHT_K> Bucket;
typedef boost::shared_ptr<Bucket> BucketSPtr;
typedef std::set< boost::shared_ptr<Bucket> > BucketContainer;
typedef std::pair<BucketSPtr,BucketSPtr> BucketSPtrPair;
typedef boost::optional<BucketSPtrPair> MaybeBuckets;
 
class AddressTree : public boost::noncopyable
{
public:

    //! The constructor initializes the container and saves a reference to the
    //! current node address
    AddressTree(const AddressData);

    //! destructor
    ~AddressTree();
 
    bool addAddress( AddressSPtr address );
    void removeAddress( AddressSPtr address );

    //! returns the number of addresses store in the container
    //! @return number of address
    size_t size() const;

protected:

    typedef boost::shared_mutex Mutex;
    typedef boost::unique_lock<Mutex> WriteLock;
    typedef boost::shared_lock<Mutex> ReadLock;
    typedef boost::upgrade_lock<Mutex> UpgradableLock;
    typedef boost::upgrade_to_unique_lock<Mutex> UpgradedWriteLock;
    mutable Mutex mutex; // RW mutex

    MaybeBuckets split( BucketContainer::const_iterator bucket_it );

    BucketContainer buckets;

    const AddressData nodeAddress;

    //! Finds the bucket containing the address space for this address.
    //! Should be called from a read-lock.
    //! @param address the address
    //! @return iterator to bucket
    BucketContainer::const_iterator findBucket( AddressData& address ) const; 

    void merge( BucketContainer::const_iterator bucket_it );
};
 
}; // dht
}; // torrentsync
