#pragma once

#include <cstdint>
#include <string>

#include <torrentsync/utils/Buffer.h>

#include <boost/optional.hpp>

namespace torrentsync
{
namespace dht
{

class NodeData;
typedef std::pair<NodeData,NodeData> Bounds;
typedef boost::optional<Bounds> MaybeBounds;

//! Basic implementation of a 160 bit data block
//! This class can't be instantiated alone, must be subclassed first.
class NodeData
{
public:

    //! NodeData constructor. Will not initialize any fields. The address 
    //! data must be initialized before usage or it will contain stack data.
    inline NodeData() {};

    //! constructor expects to receive a buffer with the binary data to parse
    NodeData(const torrentsync::utils::Buffer&);

    //! Parse a string from an hexadecimal string. useful for storing
    //! readable data in the code (and unit test).
    //! @param str the string with the hexadecimal representation [0-9A-F]{40}
    //! @return the parsed NodeData
    static inline NodeData parse( const std::string& str );

    //! Destructor
    ~NodeData();
	
    inline bool operator==( const NodeData& addr ) const;
    inline bool operator!=( const NodeData& addr ) const;
    inline bool operator< ( const NodeData& addr ) const;
    inline bool operator<=( const NodeData& addr ) const;
    inline bool operator> ( const NodeData& addr ) const;
    inline bool operator>=( const NodeData& addr ) const;

    const std::string string() const;

    static const NodeData minValue;
    static const NodeData maxValue;

	static const uint32_t ADDRESS_STRING_LENGTH;
	static const uint32_t ADDRESS_BITS;

    static MaybeBounds splitInHalf(const NodeData& low, const NodeData& high);

    static const NodeData getRandom();

    //! Parses an NodeData information from the Buffer.
    //! In this class it implements the parsing of the NodeData data.
    //! @param begin the beginning of the data in the iterator
    //! @param end the beginning of the data in the iterator
    //! @return an NodeData with the parse data
    //! @throws std::invalid_argument in case the data is not correct
    void read(
        torrentsync::utils::Buffer::const_iterator begin,
        const torrentsync::utils::Buffer::const_iterator end  );

    torrentsync::utils::Buffer write() const;

    //! amount of binary data to parse the NodeData class from binary data.
    static const size_t addressDataLength;

protected:

    void parseString( const std::string& str );

    // not using std::bitset for lack of comparing operators
    // but complicates splitInHalf
    uint64_t p1; // 0   -> 63
    uint64_t p2; // 64  -> 127
    uint32_t p3; // 128 -> 159
};

inline bool NodeData::operator==( const NodeData& addr ) const
{
    return p1 == addr.p1 &&
           p2 == addr.p2 &&
           p3 == addr.p3;
}

inline bool NodeData::operator!=( const NodeData& addr ) const
{
    return p1 != addr.p1 ||
           p2 != addr.p2 ||
           p3 != addr.p3;
}

inline bool NodeData::operator<( const NodeData& addr ) const
{
    return p1 < addr.p1 ||
           (p1 == addr.p1 && p2 < addr.p2) ||
           (p1 == addr.p1 && p2 == addr.p2 && p3 < addr.p3);
}

inline bool NodeData::operator<=( const NodeData& addr ) const
{
    return 	p1 < addr.p1 ||
            (p1 == addr.p1 && p2 < addr.p2) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 < addr.p3) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 == addr.p3);
}

inline bool NodeData::operator>( const NodeData& addr ) const
{
    return p1 > addr.p1 ||
           (p1 == addr.p1 && p2 > addr.p2) ||
           (p1 == addr.p1 && p2 == addr.p2 && p3 > addr.p3);
}

inline bool NodeData::operator>=( const NodeData& addr ) const
{
    return 	p1 > addr.p1 ||
            (p1 == addr.p1 && p2 > addr.p2) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 > addr.p3) ||
            (p1 == addr.p1 && p2 == addr.p2 && p3 == addr.p3);
}


inline NodeData NodeData::parse( const std::string& str )
{
    NodeData data;
    data.parseString(str);
    return data;
}

std::ostream& operator<<( std::ostream& out, const NodeData& data );

}; // dht
}; // torrentsync
