#pragma once

#include <boost/shared_ptr.hpp>
#include <cstring>

namespace torrentsync
{
namespace utils
{

//! Generic buffer class.
//! Useful to bring around chunks of binary data.
class Buffer
{
public:
    //! the type used to keep the data
    typedef char value_type;

    //! Constructor to initialize the data from an address and the length of
    //! the data itself
    Buffer( const char* str, const size_t size ) {
        clear();
        resize(size);
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str),size);
    }

    //! Constructor to initialize the buffer from a c string
    Buffer( const char* str ) {
        clear();
        const int size = strlen(str);
        resize(size);
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str),size);
    }

    //! Constructor to initialize the buffer from a c++ string
    Buffer( const std::string& str ) {
        clear();
        resize(str.size());
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str.c_str()),_size);
    }

    //! Initializes an empty buffer of a specific size
    Buffer( const size_t size ) { resize(size); }

    //! Initialize an empty buffer with nothing allocated
    Buffer() { clear(); };

    inline bool operator==( const Buffer& buffer ) const
    {
        if( _size != buffer._size )
            return false;
        return memcmp(_data.get(),buffer._data.get(), _size) == 0;
    }

    //! resizes the buffer keeping as much data as possible
    inline void resize( const size_t size )
    {
        value_type* ptr = new value_type[size];
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
    inline const char* begin() const { return _data.get(); }

    //! iterator to buffer's end
    inline const char* end()   const { return _data.get()+_size; }

    //! returns true if the buffer is empty
    inline bool empty()        const { return _size == 0; }

    //! returns the size of the buffer
    inline size_t size()       const { return _size; }

private:

    //! the size of the buffer
    size_t _size;

    //! pointer to the data
    boost::shared_ptr<value_type> _data;
    
};

}; // utils
}; // torrentsync
