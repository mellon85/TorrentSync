#pragma once

#include <boost/shared_ptr.hpp>
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <algorithm>

namespace torrentsync
{
namespace utils
{

//! Generic buffer class.
//! Useful to bring around chunks of binary data.
//! It will always allocate n+1+sizeof(size_t) bytes of data to hold a
//! \0 as a delimiter to ! simplify it's usage as a string container and
//! the first 8 bytes ! are the buffer length. It has been encoded in the
//!  buffer allocated ! memory to simplify the freeze functionality.
template <class value_type_t>
class BufferImpl
{
public:
    //! the type used to keep the data
    typedef value_type_t value_type;

    //! const iterator type
    typedef value_type_t const * const_iterator;

    //! iterator type
    typedef value_type_t * iterator;

private:
    //! type to embedded the size along with the data. Nice way to avoid
    //! having a second shared_ptr just to hold the size in case the copy
    //! on write is triggered.
    typedef union {
        uint8_t _raw_bytes[];
        struct {
            struct {
                size_t     size;
            } _meta;
            value_type data[];
        } _sized;
    } intrusive_size_t;

    static constexpr size_t adjust_size = sizeof(size_t);

public:
    //! returns the size of the buffer
    size_t size() const
    {
        return _data.get() ? _data->_sized.size : 0;
    }

    //! Constructor to initialize the data from an address and the length of
    //! the data itself
    BufferImpl( const char* str, const size_t size ) {
        resize(size,false);
        std::copy(str,str+size,begin());
    }

    //! Constructor to initialize the buffer from a c string
    BufferImpl( const char* str ) {
        const int size = strlen(str);
        resize(size,false);
        std::copy(str,str+size,begin());
    }

    //! Constructor to initialize the buffer from a c++ string
    BufferImpl( const std::string& str ) {
        resize(str.size(),false);
        std::copy(str.begin(),str.end(),begin());
    }

    //! Initializes an empty buffer of a specific size
    BufferImpl( const size_t size ) { resize(size,false); }

    //! Initialize an empty buffer with nothing allocated
    BufferImpl() { resize(0,false); };

    bool operator==( const BufferImpl& buffer ) const
    {
        return std::equal(cbegin(),cend(),buffer.cbegin());
    }

    //! resizes the buffer keeping as much data as possible
    void resize( const size_t new_size, const bool preserve = true)
    {   // freeze handled by resize before begin for performance
        defreeze(preserve); // in case preserve is false

        const size_t element_bytes = size()*sizeof(value_type);
        const size_t byte_size     = new_size*sizeof(value_type)+1+adjust_size;

        intrusive_size_t *ptr = reinterpret_cast<intrusive_size_t*>(new uint8_t[byte_size]);

        if( _data.get() && preserve ) // copy the old data in the new buffer
            std::copy(begin(), begin()+std::min(new_size,element_bytes),ptr->_sized.data);

        ptr->_sized.size = new_size;     // set the size
        ptr->_raw_bytes[byte_size-1] = 0; // set the end byte
        
        _data.reset(ptr);
    }

    //! returns a pointer to the data
    value_type *get() {
        return begin();
    }
    
    //! returns a pointer to the data in const context
    const value_type * const get() const {
        return cbegin();
    }

    //! deletes all the content of the buffer
    void clear() { resize(0); }

    //! iterator to buffer's start
    iterator begin() {
        defreeze(true);
        return _data->_sized.data;
    }

    //! iterator to buffer's end;
    //! freeze handled by begin
    iterator end()   {
        return begin()+size();
    }

    //! iterator to buffer's start
    const_iterator cbegin() const { return _data->_sized.data; }

    //! iterator to buffer's end
    const_iterator cend()   const { return cbegin()+size(); }

    //! returns true if the buffer is empty
    bool empty() const { return size() == 0; }

    //! returns a reference to the specific byte
    //! @throw std::out_of_range in case the data is outside the amount
    value_type& operator[]( const size_t index );

    const value_type& operator[]( const size_t index ) const;

    void copy(
        const size_t index,
        const const_iterator begin,
        const const_iterator end);

    //! The buffer is now Copy On Write.
    //! Next write operation will be performed on a different memory
    //! copy of the data. In this way copies of the buffer will remain
    //! unchanged
    void freeze()    const { frozen = true; } 

    bool is_frozen() const { return frozen; }

private:

    void defreeze( const bool keep_data ) { 
        if( !is_frozen() )
            return;

        //! a buffer must exist to be frozen
        assert( _data.get() );

        // as the buffer was frozen, we have do duplicate it and use a copy
        // from now on.

        const size_t byte_size = _data->_sized.size * sizeof(value_type) + 1 + adjust_size;
        
        intrusive_size_t *ptr = reinterpret_cast<intrusive_size_t*>(new uint8_t[byte_size]);

        if (keep_data)
        {
            std::copy(_data->_raw_bytes,
                      _data->_raw_bytes+byte_size,
                      ptr->_raw_bytes);
        }
        else
        {
            ptr->_sized.size = _data->_sized.size;
            ptr->_raw_bytes[byte_size-1] = 0; // set the end byte
        }

        _data.reset(ptr);
        frozen = false;
    }

    //! frozen status
    //! if it's frozen and a change is done a copy will be created
    mutable bool frozen = false;

    //! pointer to the data
    boost::shared_ptr<intrusive_size_t> _data;
};


template <class value_type_t>
auto BufferImpl<value_type_t>::operator[]( const size_t index ) -> value_type&
{
    // freeze handled by begin
    if (index >= size())
        throw std::out_of_range("BufferImpl out of range");
    return begin()[index];
}

template <class value_type_t>
auto BufferImpl<value_type_t>::operator[]( const size_t index ) const -> const value_type&
{
    if (index >= size())
        throw std::out_of_range("BufferImpl out of range");
    return cbegin()[index];
}

template <class value_type_t>
auto BufferImpl<value_type_t>::copy(
    const size_t index,
    const const_iterator begin,
    const const_iterator end) -> void
{
    // freeze handled by begin
    if (index+std::distance(begin,end) >= size())
        throw std::out_of_range("BufferImpl out of range");
 
    std::copy(begin(),begin,end);
}

typedef BufferImpl<char> Buffer;

inline std::ostream& operator<<( std::ostream& stream, const Buffer& buff )
{
    stream << buff.cbegin();
    return stream;
};

}; // utils
}; // torrentsync
