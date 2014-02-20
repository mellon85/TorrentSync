#pragma once

#ifdef HAS_Cxx11
#include <cstdint>
#else
#include <stdint.h>
#endif
#include <string>

#include <boost/optional.hpp>

namespace torrentsync
{
namespace dht
{

class AddressData;
typedef std::pair<AddressData,AddressData> Bounds;
typedef boost::optional<Bounds> MaybeBounds;

//! Basic implementation of a 160 bit data block
//! This class can't be instantiated alone, must be subclassed first.
class AddressData
{
public:
    AddressData(const std::string&);
    inline ~AddressData() {};
	
    inline bool operator==( const AddressData& addr ) const;
    inline bool operator!=( const AddressData& addr ) const;
    inline bool operator< ( const AddressData& addr ) const;
    inline bool operator<=( const AddressData& addr ) const;
    inline bool operator> ( const AddressData& addr ) const;
    inline bool operator>=( const AddressData& addr ) const;

    const std::string string() const;
    const std::string byteString() const;

    //! may throw std::invalid_argument
    void parse( const std::string& str );
    static AddressData parseByteString( const std::string& str );

    static const AddressData minValue;
    static const AddressData maxValue;

	static const uint32_t ADDRESS_STRING_LENGTH;
	static const uint32_t ADDRESS_BITS;

    static MaybeBounds splitInHalf(const AddressData& low, const AddressData& high);

    static const AddressData getRandom();
protected:
    inline AddressData() {};

    // not using std::bitset for lack of comparing operators
    // but complicates splitInHalf
    uint64_t p1; // 0   -> 63
    uint64_t p2; // 64  -> 127
    uint32_t p3; // 128 -> 159
};

inline bool AddressData::operator==( const AddressData& addr ) const
{
    return p1 == addr.p1 &&
           p2 == addr.p2 &&
           p3 == addr.p3;
}

inline bool AddressData::operator!=( const AddressData& addr ) const
{
    return p1 != addr.p1 ||
           p2 != addr.p2 ||
           p3 != addr.p3;
}

inline bool AddressData::operator<( const AddressData& addr ) const
{
    return p1 < addr.p1 ||
           (p1 == addr.p1 && p2 < addr.p2) ||
           (p1 == addr.p1 && p2 == addr.p2 && p3 < addr.p3);
}

inline bool AddressData::operator<=( const AddressData& addr ) const
{
    return 	p1 < addr.p1 ||
            (p1 == addr.p1 && p2 < addr.p2) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 < addr.p3) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 == addr.p3);
}

inline bool AddressData::operator>( const AddressData& addr ) const
{
    return p1 > addr.p1 ||
           (p1 == addr.p1 && p2 > addr.p2) ||
           (p1 == addr.p1 && p2 == addr.p2 && p3 > addr.p3);
}

inline bool AddressData::operator>=( const AddressData& addr ) const
{
    return 	p1 > addr.p1 ||
            (p1 == addr.p1 && p2 > addr.p2) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 > addr.p3) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 == addr.p3);
}



template <class Stream>
Stream& operator<<( Stream& out, const AddressData& data )
{
    out << data.string();
    return out;
}

}; // dht
}; // torrentsync
