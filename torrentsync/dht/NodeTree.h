#pragma once

#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/NodeBucket.h>
#include <torrentsync/dht/DHTConstants.h>

#include <memory>
#include <mutex>
#include <set>
#include <list>
#include <functional>

#include <boost/utility.hpp>

namespace torrentsync
{
namespace dht
{

typedef torrentsync::dht::NodeBucket<DHT_K> Bucket;
typedef std::shared_ptr<Bucket> BucketSPtr;
typedef std::set<std::shared_ptr<Bucket>, bool(*)(const BucketSPtr&,const BucketSPtr&) > BucketContainer;
typedef std::pair<BucketSPtr,BucketSPtr> BucketSPtrPair;
typedef boost::optional<BucketSPtrPair> MaybeBuckets;
 
/**
 * NodeTree is the tree structure to keep the known DHT addresses as per DHT 
 * specification.
 * Access to this class is thread safe as it manages it's own internal
 * consistency.
 */
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

    //! clears every bucket in the tree and resets its structure
    void clear() noexcept;

    //! find an address inside the tree
    const boost::optional<NodeSPtr> getNode(
        const NodeData& data ) const noexcept;

    //! find the closest (DHT_FIND_NODE_COUNT) addresses to this address we know
    const std::list<NodeSPtr> getClosestNodes(
        const NodeData& data) const;

    //! for each node call the function to do something with the nodes
    void for_each( std::function<void (const Node&)> );

protected:

    mutable std::recursive_mutex _mutex;

    //! splits, if possible, a bucket in 2 splitting the contents
    MaybeBuckets split( BucketContainer::const_iterator bucket_it );

    /** Finds the bucket containing the address space for this address.
     *  Should be called from a read-lock; it will always return a valid bucket.
     *  complexity: O(log n)
     *  @param address the address
     *  @return iterator to bucket
     */
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
