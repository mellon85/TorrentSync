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
 
class AddressContainer : public boost::noncopyable
{
public:

    //! The constructor initializes the container and saves a reference to the
    //! current node address
    AddressContainer(const AddressData&);

    //! destructor
    ~AddressContainer();
 
    void addAddress( AddressSPtr address );
    void removeAddress( AddressSPtr address );

    //! returns the number of addresses store in the container
    //! @return number of address
    size_t size() const;

protected:
    typedef AddressBucket<DHT_K> Bucket;
    typedef boost::shared_ptr<Bucket> BucketSPtr;

    typedef boost::shared_mutex Mutex;
    typedef boost::unique_lock<Mutex> WriteLock;
    typedef boost::shared_lock<Mutex> ReadLock;
    typedef boost::upgrade_lock<Mutex> UpgradableLock;
    typedef boost::upgrade_to_unique_lock<Mutex> UpgradedWriteLock;
    mutable Mutex mutex; // RW mutex

    typedef std::set< boost::shared_ptr<Bucket> > BucketContainer;

    BucketContainer buckets;

    const AddressData& nodeAddress;

    //! Finds the bucket containing the address space for this address.
    //! Should be called from a read-lock.
    //! @param address the address
    //! @return bucket
    BucketContainer::key_type findBucket( AddressData& address ) const; 

    void merge( BucketSPtr bucket );

    std::pair<BucketSPtr,BucketSPtr> split( BucketSPtr bucket );
};
 
}; // dht
}; // torrentsync
