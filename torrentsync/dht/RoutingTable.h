#pragma once

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>

#include <torrentsync/dht/AddressTree.h>

#include <list>

namespace torrentsync
{
namespace dht
{

using boost::asio::ip::udp;

class RoutingTable : public boost::noncopyable
{
public:
    RoutingTable( const udp::endpoint& endpoint );
    ~RoutingTable();

    inline const udp::endpoint& getEndpoint() const;

    inline boost::asio::io_service& getIO_service();

protected:
    
    //! Performs a table cleanup, usually called by a timer from boost::asio
    //! - removes bad addresses,
    //! - starts bucket refresh,
    //! - sends ping to aging nodes.
    void cleanupTable();

private:

    //! list of address to populate the table with
    std::list<Address> initial_addresses;

    //! Serialization friend class
    friend class boost::serialization::access;
    
    //! Split the serialization in 2 different steps
    BOOST_SERIALIZATION_SPLIT_MEMBER();

    template <class Archive>
    void save( Archive &ar, const unsigned int version) const;

    template <class Archive>
    void load( Archive &ar, const unsigned int version);

    //! IO service of for the routing table
    boost::asio::io_service io_service;

    //! Address of the udp endpoint of the routing table
    udp::endpoint endpoint;
};

template <class Archive>
void RoutingTable::save( Archive &ar, const unsigned int version) const
{
    // TODO
}

template <class Archive>
void RoutingTable::load( Archive &ar, const unsigned int version)
{
    // TODO 
    // refresh all the nodes
    // 1. ping all known and insert them in the routing table with the normal procedure
    // 2. perform normal startup operation and let the bucket refreshing do it's job
}

boost::asio::io_service& RoutingTable::getIo_service()
{
    return io_service;
}

const udp::endpoint& RoutingTable::getEndpoint() const
{
    return endpoint;
}

}; // dht
}; // torrentsync

BOOST_CLASS_VERSION(torrentsync::dht::RoutingTable, 0);

