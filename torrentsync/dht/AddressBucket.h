#pragma once

#include <torrentsync/dht/AddressData.h>
#include <torrentsync/dht/Address.h>
#include <boost/shared_ptr.hpp>
#include <boost/array.hpp>
#include <boost/utility.hpp>

namespace torrentsync
{
namespace dht
{

namespace
{

class AddressSharedPtrEquality
{
    const boost::shared_ptr<Address>& _storage;
public:
    AddressSharedPtrEquality(const boost::shared_ptr<Address>& s) : _storage(s) {}
    bool operator()(
            const boost::shared_ptr<Address>& a) const
    {
        assert(_storage.get());
        assert(a.get());
        return *a == *_storage;
    }

    bool operator()(
            const boost::shared_ptr<Address>& a,
            const boost::shared_ptr<Address>& b) const
    {
        assert(a.get());
        assert(b.get());
        return *a == *b;
    }
};

class AddressSharedPtrOrder
{
public:
    bool operator()(
            const boost::shared_ptr<Address>& a,
            const boost::shared_ptr<Address>& b) const
    {
        assert(a.get());
        assert(b.get());
        return *a < *b;
    }
};


class isBadPredicate
{
public:
    bool operator()( const boost::shared_ptr<Address>& addr ) const
    {
        assert(addr.get());
        return addr->isBad();
    }
};

#ifndef HAS_Cxx11
class resetSPtr
{
public:
    void operator()( boost::shared_ptr<Address>& addr ) const
    {
        assert(addr.get());
        addr.reset();
    }
};
#endif


};

//! invariants:
//!     _elements.size() <= this->maxSize()
template <size_t MaxSizeT>
class AddressBucket : public boost::noncopyable
{
public:
    AddressBucket(
            const AddressData& low,
            const AddressData& high );

    ~AddressBucket();

    typedef boost::array<boost::shared_ptr<Address>, MaxSizeT > AddressList;
    typedef typename boost::array<boost::shared_ptr<Address>, MaxSizeT >::const_iterator const_iterator;
    typedef typename boost::array<boost::shared_ptr<Address>, MaxSizeT >::iterator iterator;

    bool add(
        const boost::shared_ptr<Address>& addr);

    void remove(
        const Address& addr);

    void removeBad();

    inline size_t size()                      const { return addressCount;}
    inline size_t maxSize()                   const { return MaxSizeT;   }
    inline const AddressData& getLowerBound() const { return low;  }
    inline const AddressData& getUpperBound() const { return high; }

    void clear();

    bool canAcceptAddress(
            const boost::shared_ptr<Address>& addr ) const;

    bool inBounds(
            const boost::shared_ptr<Address>& addr ) const;

    bool operator<(const AddressBucket& bucket) const
    {
        return high < bucket.low;
    }
    
    
    inline const_iterator begin()  const { return _elements.begin(); }
    inline const_iterator end()    const { return _elements.begin()+addressCount; }
    inline const_iterator cbegin() const { return _elements.begin(); }
    inline const_iterator cend()   const { return _elements.begin()+addressCount; }
    
    
private:
    
    inline iterator begin()       { return _elements.begin(); }
    inline iterator end()         { return _elements.begin()+addressCount; }

    AddressData low;
    AddressData high;

    size_t addressCount;

    mutable AddressList _elements;

    bool canAcceptAddress(const AddressData* const addr ) const;

    bool inBounds(const AddressData* const addr ) const;

};

template <size_t MaxSizeT>
AddressBucket<MaxSizeT>::AddressBucket(
            const AddressData& low,
            const AddressData& high ) :
    low(low),
    high(high),
    addressCount(0)
{
}

template <size_t MaxSizeT>
AddressBucket<MaxSizeT>::~AddressBucket()
{
    clear();
}

template <size_t MaxSizeT>
void AddressBucket<MaxSizeT>::clear()
{
    std::for_each( begin(), end(), 
#ifdef HAS_Cxx11            
            [](boost::shared_ptr<Address>& t) { assert(t.get()); t.reset();});
#else
            resetSPtr());
#endif
    addressCount = 0;
}


// TODO should be kept in order for efficiency reason
template <size_t MaxSizeT>
bool AddressBucket<MaxSizeT>::add( const boost::shared_ptr<Address>& addr )
{
    if (!addr.get())
        throw std::invalid_argument("Address is NULL");

    if (*addr > high || *addr < low)
    {
        throw std::invalid_argument("The address can't stay in this Bucket");
    }

    if (size() == maxSize())
    {
        removeBad();
    }

    if (size() < maxSize())
    {
        _elements[addressCount++] = addr;
        return true;
    }

    return false;
}

template <size_t MaxSizeT>
void AddressBucket<MaxSizeT>::removeBad()
{
    const iterator it = std::remove_if( begin(), end(),
#ifdef HAS_Cxx11
            [](const boost::shared_ptr<Address>& addr) -> bool {
                assert(addr.get());
                return addr->isBad(); }
#else
            isBadPredicate()
#endif
            );
    addressCount = it-begin();
}

template <size_t MaxSizeT>
bool AddressBucket<MaxSizeT>::inBounds(
        const boost::shared_ptr<Address>& addr ) const
{
    if (!addr.get())
        throw std::invalid_argument("Address is NULL");
        
    return low <= *addr && *addr <= high;
}

template <size_t MaxSizeT>
bool AddressBucket<MaxSizeT>::canAcceptAddress(
        const boost::shared_ptr<Address>& addr ) const
{
    if (!addr.get())
        throw std::invalid_argument("Address is NULL");

    // try to clean the elements
    if (size() == maxSize())
    {
        if( std::find_if(begin(),end(),
#ifdef HAS_Cxx11
     [](const boost::shared_ptr<Address>& addr) -> bool {
                assert(addr.get());
                return addr->isBad(); }
#else
    isBadPredicate()
#endif
                    
                    ) == _elements.end())
        return true;
    }

    if (size() < maxSize())
    {
        return true;
    }
    return false;
}

template <size_t MaxSizeT>
void AddressBucket<MaxSizeT>::remove(
    const Address& addr)
{
    bool found = false;
    for ( size_t it = 0; it < addressCount; ++it)
    {
        const boost::shared_ptr<Address>& v = _elements[it];
        assert(v.get());

        if (addr != *v)
            continue;

        if (it+1 == addressCount) 
        {
            _elements[it] = boost::shared_ptr<Address>();
            --addressCount;
        }
        else
        {
            for( int in = it+1; in < addressCount; ++in )
            {
                _elements[it] = _elements[in];
            }
            --addressCount;
            _elements[addressCount] = boost::shared_ptr<Address>();
        }
        found = true;
    }

    if (!found)
        throw std::invalid_argument("Address not in this bucket");
}

}; // dht
}; // torrentsync

