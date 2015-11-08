#include <torrentsync/dht/NodeTree.h>

#include <exception>
#include <numeric>
#include <algorithm>

namespace torrentsync {
namespace dht {

bool sortByBounds(NodeTree::Bucket const *const x,
                  NodeTree::Bucket const *const y) noexcept {
  return x->getUpperBound() < y->getLowerBound();
}

NodeTree::NodeTree(const NodeData nodeNode)
    : _buckets(sortByBounds), _node(nodeNode) {
  clear();
}

NodeTree::~NodeTree() {
  clear();
  if (_buckets.size() > 0) {
    delete *_buckets.begin();
  }
}

bool NodeTree::addNode(NodeSPtr address) {
  if (!address)
    throw std::invalid_argument("Node is not set");

  Bucket &bucket = findBucket(*address);
  assert(bucket.inBounds(address));

  bool isAdded = bucket.add(address);
  if (!isAdded && bucket.inBounds(_node)) {
    MaybeBuckets maybe_split_buckets = split(&bucket);

    // unsplittable, ignore it
    if (!maybe_split_buckets)
      return false;

    auto &split_buckets = *maybe_split_buckets;

    assert(split_buckets.first->inBounds(address) ||
           split_buckets.second->inBounds(address));

    if (split_buckets.first->inBounds(address)) {
      isAdded = split_buckets.first->add(address);
    } else {
      assert(split_buckets.second->inBounds(address));
      isAdded = split_buckets.second->add(address);
    }
    return isAdded;
  }

  return isAdded;
}

void NodeTree::removeNode(NodeSPtr address) {
  if (!address.get())
    throw std::invalid_argument("Node is not set");

  Bucket &bucket = findBucket(*address);
  assert(bucket.inBounds(address));
  bucket.remove(*address);
}

size_t NodeTree::size() const noexcept {
  return std::accumulate(
      _buckets.begin(), _buckets.end(), static_cast<size_t>(0),
      [](const size_t init, const BucketContainer::key_type &t) -> size_t {
        return init + t->size();
      });
}

const NodeTree::Bucket &NodeTree::findBucket(const NodeData &addr) const
    noexcept {
  Bucket key(addr, addr);

  auto it = _buckets.lower_bound(&key);

  assert(it != _buckets.end());
  return **it;
}

NodeTree::Bucket &NodeTree::findBucket(const NodeData &addr) noexcept {
  return const_cast<Bucket &>(
      static_cast<const NodeTree &>(*this).findBucket(addr));
}

NodeTree::MaybeBuckets NodeTree::split(Bucket *bucket) {
  assert(bucket != nullptr);

  MaybeBounds bounds =
      NodeData::splitInHalf(bucket->getLowerBound(), bucket->getUpperBound());

  if (!bounds)
    return MaybeBuckets();

  Bucket *lower_bucket = new Bucket(bucket->getLowerBound(), bounds->first);
  Bucket *upper_bucket = new Bucket(bounds->second, bucket->getUpperBound());

  try {
    // split addresses in buckets
    for (auto it : *bucket) {
      if (lower_bucket->inBounds(it)) {
        lower_bucket->add(it);
      } else {
        assert(upper_bucket->inBounds(it));
        upper_bucket->add(it);
      }
    }

    assert(upper_bucket->size() + lower_bucket->size() == bucket->size());

    _buckets.erase(bucket);
    delete bucket;
    _buckets.insert(upper_bucket);
    _buckets.insert(lower_bucket);
  } catch (...) {
    // bucket was not removed
    delete lower_bucket;
    delete upper_bucket;
    throw;
  }

  return MaybeBuckets(std::make_pair(lower_bucket, upper_bucket));
}

void NodeTree::clear() noexcept {
  for (auto it : _buckets) {
    delete it;
  }
  _buckets.clear();

  // initialize first bucket
  Bucket *bucket = new Bucket(NodeData::minValue, NodeData::maxValue);
  try {
    _buckets.insert(bucket);
  } catch (...) {
    delete bucket;
    throw;
  }
}

const boost::optional<NodeSPtr> NodeTree::getNode(const NodeData &data) const
    noexcept {
  return findBucket(data).find(data);
}

// This method will not return the exact 8 closest, but fetches the nodes from
// three buckets, the right one and the 2 adjacent ones.
// In case we have a lot of nodes it will be precise, if not then we can't
// expect to have a precise result anyway.
// TODO this is ugly. the nodes should be sorted by distance
const std::vector<NodeSPtr>
NodeTree::getClosestNodes(const NodeData &data) const {
  std::vector<NodeSPtr> nodes;
  nodes.reserve(DHT_FIND_NODE_COUNT);
  std::set<NodeSPtr, std::function<bool(const NodeSPtr &, const NodeSPtr &)>>
  knownNodes([&data](const NodeSPtr &x, const NodeSPtr &y) {
    return (*x ^ data) <= (*y ^ data);
  });

  // find bucket
  const Bucket &bucket = findBucket(data);

  // check if we have a perfect match
  const Bucket::const_iterator perfect_match =
      std::find_if(std::begin(bucket), std::end(bucket),
                   [&data](const NodeSPtr &a) { return *a == data; });

  if (perfect_match != std::end(bucket)) {
    nodes.push_back(*perfect_match);
    return nodes;
  }

  // in case a perfect match is not found the closest nodes are returned
  auto it = _buckets.find(const_cast<Bucket *>(&bucket));
  if (it != _buckets.begin())
    --it; // put it at the left bucket

  // get the left, central and right bucket
  for (int i = 0; i < 3 && it != _buckets.end(); ++i, ++it) {
    for (auto addr : **it) {
      knownNodes.insert(addr);
    }
  }

  std::copy(knownNodes.begin(), knownNodes.end(), std::back_inserter(nodes));
  if (nodes.size() > DHT_FIND_NODE_COUNT) {
    nodes.resize(DHT_FIND_NODE_COUNT);
  }
  return nodes;
}

const NodeData &NodeTree::getTableNode() const noexcept { return _node; }

size_t NodeTree::getBucketsCount() const noexcept { return _buckets.size(); }

}; // dht
}; // torrentsync
