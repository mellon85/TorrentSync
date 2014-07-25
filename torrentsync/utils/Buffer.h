#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/call_traits.hpp>

#include <cctype>
#include <cstring>
#include <stdexcept>
#include <cassert>
#include <algorithm>

namespace torrentsync
{
namespace utils
{

/*! Generic buffer class.
 *  Useful to bring around chunks of binary data.  It will always
 *  allocate n+1+sizeof(size_t) bytes of data to hold a c-string
 *  terminator ('\0') as a delimiter to ! simplify it's usage as a
 *  string container and the first 8 bytes ! are the buffer length. It
 *  has been encoded in the buffer allocated ! memory to simplify the
 *  freeze functionality.  Can contain only POD data. I didn't develop
 *  it tobe safe if exceptions are thrown in the value_type type.
 */
template <class value_type_t>
class BufferImpl
{
public:
    typedef typename boost::call_traits<value_type_t>::value_type value_type;

    typedef typename boost::call_traits<value_type_t>::reference reference;

    typedef typename boost::call_traits<value_type_t>::const_reference const_reference;

    typedef typename boost::call_traits<value_type_t>::param_type param_type;

    //! const iterator type
    typedef value_type const * const_iterator;

    //! iterator type
    typedef value_type * iterator;

private:
    /*!
     * Contains the meta information of the buffer. The allocated size
     * and if it's immutable or not.
     */
    typedef struct {
        size_t size;
        bool frozen;
    } meta_t;

    /*! 
     * Type to embedded the size along with the data. Nice way to avoid
     * having a second shared_ptr just to hold the size in case the copy
     * on write is triggered.
     */
    typedef union {
        uint8_t _raw_bytes[];
        struct {
            meta_t _meta;
            value_type data[];
        } _sized;
    } intrusive_size_t;

    static constexpr size_t adjust_size = sizeof(meta_t);

public:

    //! returns the size of the buffer
    size_t size() const noexcept
    {
        return _data.get() ? _data->_sized._meta.size : 0;
    }

    //! In case the code is trying to use a const buffer as non const 
    //! the constructor will copy the data.
    BufferImpl( const BufferImpl &t )
    {
        resize(t.size(),false);
        std::copy(t.cbegin(),t.cend(),begin());
    }

    //! In case the code is trying to use a const buffer as non const 
    //! the constructor will copy the data.
    BufferImpl( BufferImpl &t ) { _data = t._data; }

    //! Constructor to initialize the data from an address and the length of
    //! the data itself
    BufferImpl( const param_type* data, const size_t size ) {
        resize(size,false);
        std::copy(data,data+size,begin());
    }

    //! Constructor to initialize the buffer copying a c string until a
    //! 0 is found
    BufferImpl( const char* const str ) {
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
    BufferImpl( size_t size ) { resize(size,false); }

    //! Initialize an empty buffer with nothing allocated
    BufferImpl() { resize(0,false); };

    bool operator==( const BufferImpl& buffer ) const noexcept
    {
        return std::equal(cbegin(),cend(),buffer.cbegin());
    }

    //! resizes the buffer keeping as much data as possible
    void resize( size_t new_size, bool preserve = true)
    {   // freeze handled by resize before begin for performance
        defreeze(preserve); // in case preserve is false

        const size_t element_bytes = size()*sizeof(value_type);
        const size_t byte_size     = new_size*sizeof(value_type)+1+adjust_size;

        intrusive_size_t *ptr =
            reinterpret_cast<intrusive_size_t*>(new uint8_t[byte_size]);

        if( _data.get() && preserve ) // copy the old data in the new buffer
            std::copy(
                begin(),
                begin()+std::min(new_size,element_bytes),
                ptr->_sized.data);

        ptr->_sized._meta.size = new_size; // set the size
        ptr->_sized._meta.frozen = false;  // a new buffer is not frozen
        ptr->_raw_bytes[byte_size-1] = 0;  // set the end byte

        _data.reset(ptr);
    }

    //! returns a pointer to the data
    value_type *get() {
        return begin();
    }

    //! returns a pointer to the data in const context
    const value_type * const get() const noexcept {
        return cbegin();
    }

    //! deletes all the content of the buffer
    void clear() { resize(0); }

    //! iterator to buffer's start
    iterator begin() {
        assert(_data.get());
        defreeze(true);
        return _data->_sized.data;
    }

    //! iterator to buffer's end;
    //! freeze handled by begin
    iterator end()   {
        return begin()+size();
    }

    //! iterator to buffer's start
    const_iterator cbegin() const noexcept {
        assert(_data.get());
        return _data->_sized.data;
    }

    //! iterator to buffer's end
    const_iterator cend()   const noexcept {
        return cbegin()+size();
    }

    //! returns true if the buffer is empty
    bool empty() const noexcept            { return size() == 0; }

    //! returns a reference to the specific byte
    //! @throw std::out_of_range in case the data is outside the amount
    value_type& operator[]( size_t index );

    const value_type& operator[]( size_t index ) const;

    /*! The buffer is now Copy On Write.
     *  Next write operation will be performed on a different memory
     *  copy of the data. In this way copies of the buffer will remain
     *  unchanged.
     */
    void freeze() const noexcept {
        assert(_data.get());
        _data->_sized._meta.frozen = true;
    }

    bool is_frozen() const noexcept {
        return _data.get() ?  _data->_sized._meta.frozen : 0;
    }

private:

    void defreeze( bool keep_data ) {
        if( !is_frozen() )
            return;

        assert(_data.get());
        // as the buffer was frozen, we have do duplicate it and use a copy
        // from now on.
        const size_t byte_size =
            _data->_sized._meta.size * sizeof(value_type) + 1 + adjust_size;

        intrusive_size_t *ptr =
            reinterpret_cast<intrusive_size_t*>(new uint8_t[byte_size]);

        if (keep_data)
        {
            std::copy(_data->_raw_bytes,
                      _data->_raw_bytes+byte_size,
                      ptr->_raw_bytes);
        }
        else
        {
            ptr->_sized._meta.size = _data->_sized._meta.size;
            ptr->_raw_bytes[byte_size-1] = 0; // set the end byte
        }

        _data.reset(ptr);
        ptr->_sized._meta.frozen = false;
    }

    //! pointer to the data
    boost::shared_ptr<intrusive_size_t> _data;
};

template <class value_type_t>
auto BufferImpl<value_type_t>::operator[]( size_t index ) -> value_type&
{
    // freeze handled by begin
    if (index >= size())
        throw std::out_of_range("BufferImpl out of range");
    return begin()[index];
}

template <class value_type_t>
auto BufferImpl<value_type_t>::operator[]( size_t index ) const -> const value_type&
{
    if (index >= size())
        throw std::out_of_range("BufferImpl out of range");
    return cbegin()[index];
}

typedef BufferImpl<char> Buffer;

template <class T>
std::ostream& operator<<( std::ostream& stream, const BufferImpl<T>& buff )
{
    for( auto it = buff.cbegin(); it != buff.cend(); ++it )
        stream << *it;
    return stream;
};

}; // utils
}; // torrentsync

//! Pretty printing wrapper for a buffer containing string data.
//! Will print all the content of the buffer escaping the binary data
//! until a c-string terminator ('\0') is found.
struct pretty_print
{
    const torrentsync::utils::Buffer& _buff;
    pretty_print(const torrentsync::utils::Buffer& buff) : _buff(buff) {}
};

std::ostream& operator<<( std::ostream& stream, const pretty_print& buff );
