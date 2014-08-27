#include <torrentsync/dht/NodeTree.h>

#include <exception>
#include <numeric>
#include <functional>

namespace torrentsync
{
namespace dht
{

bool sortByBounds(const BucketSPtr& x, const BucketSPtr& y) noexcept
{
    return x->getUpperBound() < y->getLowerBound();
}

NodeTree::NodeTree(
    const NodeData nodeNode) :
        _buckets(sortByBounds),
        _node(nodeNode)
{
    clear();
}

NodeTree::~NodeTree()
{
}

bool NodeTree::addNode( NodeSPtr address )
{
    if (!address.get())
        throw std::invalid_argument("Node is not set");

    std::lock_guard<std::mutex> lock(mutex);

    BucketContainer::const_iterator bucket_it = findBucket(*address);
    assert(bucket_it != _buckets.end());

    BucketContainer::key_type bucket = *bucket_it;
    assert(bucket->inBounds(address));

    bool isAdded = bucket->add(address);
    if ( !isAdded && bucket->inBounds(_node) )
    {
        MaybeBuckets maybe_split_buckets = split(bucket_it);

        // unsplittable, ignore it
        if (!maybe_split_buckets)
            return false;

        BucketSPtrPair& split_buckets = *maybe_split_buckets;

        assert(split_buckets.first->inBounds(address) ||
               split_buckets.second->inBounds(address));

        if (split_buckets.first->inBounds(address))
        {
            isAdded = split_buckets.first->add(address);
        }
        else
        {
            assert(split_buckets.second->inBounds(address));
            isAdded = split_buckets.second->add(address);
        }
        return isAdded;
    }

    return isAdded;
}

void NodeTree::removeNode( NodeSPtr address )
{
    if (!address.get())
        throw std::invalid_argument("Node is not set");

    std::lock_guard<std::mutex> lock(mutex);

    BucketContainer::const_iterator bucket_it = findBucket(*address);

    BucketContainer::key_type bucket = *bucket_it;
    assert(bucket->inBounds(address));

    bucket->remove(*address);
}

size_t NodeTree::size() const noexcept
{
    std::lock_guard<std::mutex> lock(mutex);
    return std::accumulate(_buckets.begin(), _buckets.end(), static_cast<size_t>(0),
        [](const size_t init,const BucketContainer::key_type& t) -> size_t
            { return init+t->size(); });
}

BucketContainer::const_iterator
NodeTree::findBucket( const NodeData& addr ) const
{
    const BucketContainer::key_type key(new Bucket(addr,addr));

    auto it = _buckets.lower_bound(key);

    assert( it != _buckets.end() );
    assert( it->get() );
    return it;
}

MaybeBuckets NodeTree::split( BucketContainer::const_iterator bucket_it )
{
    assert(bucket_it != _buckets.end());
    assert(bucket_it->get());
    BucketSPtr bucket = *bucket_it;

    MaybeBounds bounds = NodeData::splitInHalf(
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
 
    _buckets.erase(bucket_it);
    _buckets.insert(upper_bucket);
    _buckets.insert(lower_bucket);

    return MaybeBuckets(BucketSPtrPair(lower_bucket,upper_bucket));
}

void NodeTree::clear() noexcept
{
    std::lock_guard<std::mutex> lock(mutex);
    _buckets.clear();

    // initialize first bucket
    std::shared_ptr<Bucket> bucket(
            new Bucket( NodeData::minValue, NodeData::maxValue));
    _buckets.insert(bucket);
}

const boost::optional<NodeSPtr> NodeTree::getNode(
    const NodeData& data ) const noexcept
{
    return (*findBucket(data))->find(data); 
}

// This method will not return the exact 8 closest, but fetches the nodes from
// three buckets, the right one and the 2 adjacent ones.
// In case we have a lot of nodes it will be precise, if not then we can't 
// expect to have a precise result anyway.
const std::list<NodeSPtr> NodeTree::getClosestNodes(
    const NodeData& data) const
{
    std::list<NodeSPtr> nodes;
    std::lock_guard<std::mutex> lock(mutex);
    
    std::set<NodeSPtr,std::function<bool(const NodeSPtr&,const NodeSPtr&)> > knownNodes(
        [&data]( const NodeSPtr& x, const NodeSPtr& y) {
            return (*x ^ data) <= (*y ^ data);
        });

    // find bucket
    auto bucket_it = findBucket(data);
    
    // check if we have a perfect match
    const Bucket::const_iterator perfect_match = std::find_if(
        (*bucket_it)->cbegin(),(*bucket_it)->cend(),
            [&data](const NodeSPtr& a) { return *a == data; } );
    
    if ( perfect_match != (*bucket_it)->cend() )
    {
        nodes.push_back(*perfect_match);
        return nodes;
    }
    
    // in case a perfect match is not found the closest nodes are returned
    if (bucket_it != _buckets.begin())
        --bucket_it; // put it at the left bucket

    for( int i = 0; i < 3 && bucket_it != _buckets.end(); ++i, ++bucket_it )
    {
        for( auto it = (*bucket_it)->cbegin(); it != (*bucket_it)->cend(); ++it)
        {
            knownNodes.insert(*it);
        }
    }

    auto it = knownNodes.begin();
    for( size_t __i = 0; __i < knownNodes.size() && __i < DHT_FIND_NODE_COUNT;  ++__i, ++it )
    {
        nodes.push_back(*it);
    }
    return nodes;
}

const NodeData& NodeTree::getTableNode() const noexcept
{
    return _node;
}

size_t NodeTree::getBucketsCount() const noexcept
{
    return _buckets.size();
}

}; // dht
}; // torrentsync
