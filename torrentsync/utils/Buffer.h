#pragma once

#include <boost/shared_ptr.hpp>
#include <cstring>

namespace torrentsync
{
namespace utils
{


class Buffer
{
public:
    typedef char value_type;

    Buffer( const char* str, const size_t size ) {
        clear();
        resize(size);
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str),size);
    }

    Buffer( const char* str ) {
        clear();
        const int size = strlen(str);
        resize(size);
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str),size);
    }

    Buffer( const std::string& str ) {
        clear();
        resize(str.size());
        std::memcpy(
            static_cast<void*>(_data.get()),
            static_cast<const void*>(str.c_str()),_size);
    }

    Buffer( const size_t size ) { resize(size); }
    Buffer() { clear(); };

    inline bool operator==( const Buffer& buffer ) const
    {
        if( _size != buffer._size )
            return false;
        return memcmp(_data.get(),buffer._data.get(), _size) == 0;
    }


    inline void resize( const size_t size )
    {
        value_type* ptr = new value_type[size];
        if( _data.get() )
            std::memcpy(
                static_cast<void*>(ptr),
                static_cast<void*>(_data.get()),_size);
        _size = size;
        _data.reset(ptr);
    }

    inline value_type *get() {
        return _data.get();
    }

    inline const value_type * const get() const {
        return _data.get();
    }

    inline void clear() { _data.reset(); _size = 0; }

    inline const char* begin() const { return _data.get(); }
    inline const char* end()   const { return _data.get()+_size; }

    inline bool empty()        const { return _size == 0; }
    inline size_t size()       const { return _size; }

private:

    size_t _size;

    boost::shared_ptr<value_type> _data;
    
};

}; // utils
}; // torrentsync
