#pragma once
 
#include <torrentsync/dht/Address.h>
#include <torrentsync/dht/AddressBucket.h>
#include <torrentsync/dht/DHTConstants.h>
#include <torrentsync/common/Lock.h>
 
#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>
 
#include <set>
 
namespace torrentsync
{
namespace dht
{
typedef torrentsync::dht::AddressBucket<DHT_K> Bucket;
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
 
    //! Add an address to the tree
    bool addAddress( AddressSPtr address );

    //! Removes an address
    void removeAddress( AddressSPtr address );

    //! Returns the number of addresses stored in the container
    //! @return number of address.
    size_t size() const;

    //! Returns our own address used to setup the tree
    const AddressData& getNodeAddress() const { return nodeAddress; }

    // clears every bucket
    void clear();

    // find an address inside the tree
    const boost::optional<AddressSPtr> getAddress(
           const AddressData& data ) const;

protected:

    mutable Mutex mutex; // RW mutex

    //! splits, if possible, a bucket in 2 splitting the contents
    MaybeBuckets split( BucketContainer::const_iterator bucket_it );

    //! Finds the bucket containing the address space for this address.
    //! Should be called from a read-lock.
    //! @param address the address
    //! @return iterator to bucket
    BucketContainer::const_iterator findBucket(
        const AddressData& address ) const; 

    //! return sthe counts of the buckets
    const size_t getBucketsCount() const { return buckets.size(); }

private:
    //! bucket container
    BucketContainer buckets;

    //! address used as the center of the tree
    const AddressData nodeAddress;
};

}; // dht
}; // torrentsync
