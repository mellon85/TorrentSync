#pragma once

#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/NodeBucket.h>
#include <torrentsync/dht/DHTConstants.h>
#include <torrentsync/utils/Lock.h>

#include <boost/shared_ptr.hpp>
#include <boost/utility.hpp>

#include <set>

namespace torrentsync
{
namespace dht
{
typedef torrentsync::dht::NodeBucket<DHT_K> Bucket;
typedef boost::shared_ptr<Bucket> BucketSPtr;
typedef std::set< boost::shared_ptr<Bucket> > BucketContainer;
typedef std::pair<BucketSPtr,BucketSPtr> BucketSPtrPair;
typedef boost::optional<BucketSPtrPair> MaybeBuckets;
 
// @TODO must be reritten to use Node instead of NodeData
// and so renamed NodeTree.
class NodeTree : public boost::noncopyable
{
public:

    //! The constructor initializes the container and saves a reference to the
    //! current node address
    NodeTree(const NodeData);

    //! destructor
    ~NodeTree();
 
    //! Add an address to the tree
    bool addNode( NodeSPtr address );

    //! Removes an address
    void removeNode( NodeSPtr address );

    //! Returns the number of addresses stored in the container
    //! @return number of address.
    size_t size() const noexcept;

    //! Returns our own address used to setup the tree
    const NodeData& getTableNode() const noexcept;

    // clears every bucke
    void clear() noexcept;

    // find an address inside the tree
    const boost::optional<NodeSPtr> getNode(
           const NodeData& data ) const;

protected:

    mutable Mutex mutex; // RW mutex

    //! splits, if possible, a bucket in 2 splitting the contents
    MaybeBuckets split( BucketContainer::const_iterator bucket_it );

    //! Finds the bucket containing the address space for this address.
    //! Should be called from a read-lock.
    //! @param address the address
    //! @return iterator to bucket
    BucketContainer::const_iterator findBucket(
        const NodeData& address ) const; 

    //! returns the counts of the buckets
    size_t getBucketsCount() const noexcept;

private:
    //! bucket container
    BucketContainer _buckets;

    //! address used as the center of the tree
    const NodeData _node;
};

}; // dht
}; // torrentsync
