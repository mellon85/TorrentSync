#pragma once

#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>

#include <memory>
#include <boost/array.hpp>
#include <boost/utility.hpp>
#include <boost/optional.hpp>

#include <algorithm>

namespace torrentsync
{
namespace dht
{

/** Bucket containing nodes
 * A bucket container for node data, used as a node in NodeTree.
 */
template <size_t MaxSizeT>
class NodeBucket : public boost::noncopyable
{
public:
    //! constructor
    //! @param low the lower bound
    //! @param high the higher bound
    NodeBucket(
        const NodeData& low,
        const NodeData& high );

    ~NodeBucket();

    typedef boost::array<std::shared_ptr<Node>, MaxSizeT > NodeList;
    typedef typename NodeList::const_iterator const_iterator;
    typedef typename NodeList::iterator iterator;

    /** adds a node to the bucket
     * In case the bucket it's already full it will try to remove older bad nodes.
     * @param addr the node
     * @return true if the node was added, false if it was not possible
     * @throws std::invalid_argument in case addr is not in the bucket bounds
     */
    bool add(
        const std::shared_ptr<Node> addr);

    /** remove a node from the bucket
     * In case the bucket it's already full it will try to remove older bad nodes.
     * @param addr the node
     * @return true if it was removed, false if not found
     */
    bool remove(
        const Node& addr);

    /** Looks for a node in the bucket
     * @return a valid optional value if found or nothing
     * @param addr the address
     */
    const boost::optional<NodeSPtr> find(
        const NodeData& addr) const;

    void removeBad();

    inline size_t size()    const;
    inline size_t maxSize() const;

    void clear();

    bool inBounds(
        const std::shared_ptr<Node>& addr ) const;
    bool inBounds(
        const NodeData& addr ) const;

    inline const NodeData& getLowerBound() const { return low;  };
    inline const NodeData& getUpperBound() const { return high; };

    inline const_iterator begin()  const { return cbegin(); }
    inline const_iterator end()    const { return cend(); }

    inline const_iterator cbegin() const { return _elements.begin(); }
    inline const_iterator cend()   const { return _elements.begin()+addressCount; }

    inline iterator begin()       { return _elements.begin(); }
    inline iterator end()         { return _elements.begin()+addressCount; }

    std::ostream& string( std::ostream& out ) const;

private:

    NodeData low;
    NodeData high;

    size_t addressCount;

    mutable NodeList _elements;
};

template <size_t MaxSizeT>
NodeBucket<MaxSizeT>::NodeBucket(
            const NodeData& low,
            const NodeData& high ) :
    low(low), high(high),
    addressCount(0)
{
}

template <size_t MaxSizeT>
NodeBucket<MaxSizeT>::~NodeBucket()
{
    clear();
}

template <size_t MaxSizeT>
size_t NodeBucket<MaxSizeT>::size() const
{
    return addressCount;
}

template <size_t MaxSizeT>
size_t NodeBucket<MaxSizeT>::maxSize() const
{
    return MaxSizeT;
}

template <size_t MaxSizeT>
void NodeBucket<MaxSizeT>::clear()
{
    std::for_each( begin(), end(), 
            [](std::shared_ptr<Node>& t) { assert(t.get()); t.reset();});
    addressCount = 0;
}

template <size_t MaxSizeT>
bool NodeBucket<MaxSizeT>::add( const std::shared_ptr<Node> addr )
{
    assert(addr);
    if (*addr > high || *addr < low)
    {
        throw std::invalid_argument("The address can't stay in this Bucket: "
            + addr->string() + " "+low.string()+"-"+high.string());
    }

    if (size() == maxSize())
    {
        removeBad();
    }

    if (size() < maxSize())
    {
        // find first greater than the current address
        iterator it = std::find_if( begin(), end(),
            bind1st(std::less<std::shared_ptr<Node> >(),addr) );
        std::copy_backward(it,end(),end()+1);
        *it = addr;
        ++addressCount;
        return true;
    }

    return false;
}

template <size_t MaxSizeT>
void NodeBucket<MaxSizeT>::removeBad()
{
    const iterator it = std::remove_if( begin(), end(),
            [](const std::shared_ptr<Node>& addr) -> bool {
                assert(addr.get());
                return addr->isBad(); }
            );
    addressCount = it-begin();
}

template <size_t MaxSizeT>
bool NodeBucket<MaxSizeT>::inBounds(
        const std::shared_ptr<Node>& addr ) const
{
    assert(addr.get());
    return inBounds(*addr);
}

template <size_t MaxSizeT>
bool NodeBucket<MaxSizeT>::inBounds(
        const NodeData& addr ) const
{
    return low <= addr && addr <= high;
}

template <size_t MaxSizeT>
bool NodeBucket<MaxSizeT>::remove(
    const Node& addr)
{
    for ( size_t it = 0; it < addressCount; ++it)
    {
        const std::shared_ptr<Node>& v = _elements[it];
        assert(v.get());

        if (addr == *v)
        {
            if (it+1 == addressCount) 
            {
                _elements[it] = std::shared_ptr<Node>();
                --addressCount;
            }
            else
            {
                std::copy( begin()+it+1, end(), begin()+it);
                --addressCount;
                _elements[addressCount] = std::shared_ptr<Node>();
            }
            return true;
        }
    }
    return false;
}

template <size_t MaxSizeT>
std::ostream& operator<<( std::ostream& out, const NodeBucket<MaxSizeT>& element )
{
    return element.string(out);
}

template <size_t MaxSizeT>
std::ostream& NodeBucket<MaxSizeT>::string( std::ostream& out ) const
{
    out << low.string() << ' ' << high.string() << std::endl;
    for( const_iterator it = cbegin(); it != cend(); ++it )
    {
        assert(it->get());
        out << **it << std::endl;
    }
    return out;
}

template <size_t MaxSizeT>
const boost::optional<NodeSPtr> NodeBucket<MaxSizeT>::find(
    const NodeData& addr) const
{
    const_iterator it = std::find_if( begin(), end(), 
            [addr](const std::shared_ptr<Node>& bucket_addr) -> bool {
                assert(bucket_addr.get());
                return static_cast<NodeData&>(*bucket_addr) == addr; }
            );

    boost::optional<NodeSPtr> ret;
    if ( it != end() )
    {
        ret = *it;
    }
    return ret;
}

}; // dht
}; // torrentsync
