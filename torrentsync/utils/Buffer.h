#pragma once

#include <boost/shared_ptr.hpp>
#include <cstring>
#include <stdexcept>
#include <cassert>

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

    //! returns the size of the buffer
    size_t size() const
    {
        return _data.get() 
            ? *(reinterpret_cast<size_t*>(_data.get()))
            : 0;
    }

    //! Constructor to initialize the data from an address and the length of
    //! the data itself
    BufferImpl( const char* str, const size_t size ) {
        resize(size,false);
        std::memcpy(
            begin(),
            static_cast<const void*>(str),size);
    }

    //! Constructor to initialize the buffer from a c string
    BufferImpl( const char* str ) {
        const int size = strlen(str);
        resize(size,false);
        std::memcpy(
            begin(),
            static_cast<const void*>(str),size);
    }

    //! Constructor to initialize the buffer from a c++ string
    BufferImpl( const std::string& str ) {
        resize(str.size(),false);
        std::memcpy(
            begin(),
            static_cast<const void*>(str.c_str()),str.size());
    }

    //! Initializes an empty buffer of a specific size
    BufferImpl( const size_t size ) { resize(size,false); }

    //! Initialize an empty buffer with nothing allocated
    BufferImpl() { resize(0,false); };

    bool operator==( const BufferImpl& buffer ) const
    {
        return memcmp(_data.get(),buffer._data.get(), size()+sizeof(size_t)) == 0;
    }

    //! resizes the buffer keeping as much data as possible
    void resize( const size_t new_size, const bool preserve = true)
    {   // freeze handled by resize before begin for performance
        defreeze(preserve); // in case preserve is false
        const size_t _size = size();
        void* ptr = new uint8_t[sizeof(value_type)*new_size+1+sizeof(size_t)];
        //! TODO should set to 0 only the longer part
        memset(static_cast<uint8_t*>(ptr)+sizeof(size_t),0,(new_size) * sizeof(value_type)+1);
        if( _data.get() && preserve )
            std::memcpy(static_cast<uint8_t*>(ptr)+sizeof(size_t),begin(),
                    new_size < _size ? new_size : _size );
        *(static_cast<size_t*>(ptr)) = new_size;
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

        return 
            reinterpret_cast<iterator>(
                static_cast<uint8_t*>(_data.get())+sizeof(size_t)); }

    //! iterator to buffer's end;
    iterator end()   {
        //! freeze handled by begin
        return begin()+size(); }

    //! iterator to buffer's start
    const_iterator cbegin() const {
        return 
            reinterpret_cast<const_iterator>(
                static_cast<uint8_t*>(_data.get())+sizeof(size_t)); }

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
        const value_type* const data,
        const size_t length );

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

        //! modify a different object
        const size_t same_size = size()+sizeof(size_t);
        void* ptr = new uint8_t[sizeof(value_type)*same_size+1+sizeof(size_t)];
        if (keep_data)
            std::memcpy(ptr,_data.get(),same_size);
        else
            *(static_cast<size_t*>(ptr)) = same_size;

        _data.reset(ptr);
        frozen = false;
    }

    //! frozen status
    //! if it's frozen and a change is done a copy will be created
    mutable bool frozen = false;

    //! pointer to the data
    boost::shared_ptr<void> _data;
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
    const value_type* const data,
    const size_t length ) -> void
{
    // freeze handled by begin
    if (index+length >= size())
        throw std::out_of_range("BufferImpl out of range");
 
    memcpy(static_cast<void*>(begin()+index),
           static_cast<const void* const>(data), length);
}

typedef BufferImpl<char> Buffer;

inline std::ostream& operator<<( std::ostream& stream, const Buffer& buff )
{
    stream << buff.cbegin();
    return stream;
};

}; // utils
}; // torrentsync
