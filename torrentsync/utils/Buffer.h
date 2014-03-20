#pragma once

#include <boost/shared_ptr.hpp>
#include <cstring>
#include <stdexcept>

namespace torrentsync
{
namespace utils
{


//! Generic buffer class.
//! Useful to bring around chunks of binary data.
//! It will always allocate n+1 bytes of data to hold a \0 as a delimiter to
//! simplify it's usage as a string container.
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

    //! Constructor to initialize the data from an address and the length of
    //! the data itself
    BufferImpl( const char* str, const size_t size ) {
        clear();
        resize(size);
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str),size);
    }

/*
    BufferImpl( BufferImpl<value_type_t>&& move )
    {
        _data = move._data;
        _size = move._size;
        move._data.reset();
    }
*/

    //! Constructor to initialize the buffer from a c string
    BufferImpl( const char* str ) {
        clear();
        const int size = strlen(str);
        resize(size);
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str),size);
    }

    //! Constructor to initialize the buffer from a c++ string
    BufferImpl( const std::string& str ) {
        clear();
        resize(str.size());
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str.c_str()),_size);
    }

    //! Initializes an empty buffer of a specific size
    BufferImpl( const size_t size ) { resize(size); }

    //! Initialize an empty buffer with nothing allocated
    BufferImpl() { clear(); };

    inline bool operator==( const BufferImpl& buffer ) const
    {
        if( _size != buffer._size )
            return false;
        return memcmp(_data.get(),buffer._data.get(), _size) == 0;
    }

    //! resizes the buffer keeping as much data as possible
    inline void resize( const size_t size )
    {
        value_type* ptr = new value_type[size+1];
        memset(ptr,0,(size+1) * sizeof(value_type));
        if( _data.get() )
            std::memcpy(
                static_cast<void*>(ptr),
                static_cast<void*>(_data.get()),
                    size < _size ? size : _size );
        _size = size;
        _data.reset(ptr);
    }

    //! returns a pointer to the data
    inline value_type *get() {
        return _data.get();
    }
    
    //! returns a pointer to the data in const context
    inline const value_type * const get() const {
        return _data.get();
    }

    //! deletes all the content of the buffer
    inline void clear() { _data.reset(); _size = 0; }

    //! iterator to buffer's start
    inline iterator begin() { return _data.get(); }

    //! iterator to buffer's end;
    inline iterator end()   { return _data.get()+_size; }

    //! iterator to buffer's start
    inline const_iterator cbegin() const { return _data.get(); }

    //! iterator to buffer's end
    inline const_iterator cend()   const { return _data.get()+_size; }

    //! returns true if the buffer is empty
    inline bool empty()        const { return _size == 0; }

    //! returns the size of the buffer
    inline size_t size()       const { return _size; }

    //! returns a reference to the specific byte
    //! @throw std::out_of_range in case the data is outside the amount
    inline value_type& operator[]( const size_t index );

    inline const value_type& operator[]( const size_t index ) const;

    inline void copy( const size_t index,
                      const value_type* const data,
                      const size_t length );

private:

    //! the size of the buffer
    size_t _size;

    //! pointer to the data
    boost::shared_ptr<value_type> _data;
    
};


template <class value_type_t>
auto BufferImpl<value_type_t>::operator[]( const size_t index ) -> value_type&
{
    if (index >= _size)
        throw std::out_of_range("BufferImpl out of range");
    return _data.get()[index];
}

template <class value_type_t>
auto BufferImpl<value_type_t>::operator[]( const size_t index ) const -> const value_type&
{
    if (index >= _size)
        throw std::out_of_range("BufferImpl out of range");
    return _data.get()[index];
}

template <class value_type_t>
auto BufferImpl<value_type_t>::copy(
    const size_t index,
    const value_type* const data,
    const size_t length ) -> void
{
    if (index+length >= _size)
        throw std::out_of_range("BufferImpl out of range");
 
    memcpy(static_cast<void*>(_data.get()+index),
           static_cast<const void* const>(data), length);
}

typedef BufferImpl<char> Buffer;

}; // utils
}; // torrentsync
