#pragma once

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/udp.hpp>

#include <torrentsync/dht/AddressTree.h>
#include <torrentsync/utils/Lock.h>

#include <list>

namespace torrentsync
{
namespace dht
{

using boost::asio::ip::udp;

class RoutingTable : public boost::noncopyable
{
public:
    //! Constructor
    //! @param endpoint the port and address to bind
    RoutingTable(
        const udp::endpoint& endpoint );

    ~RoutingTable();

    const udp::endpoint getEndpoint() const;

    boost::asio::io_service& getIO_service();

protected:

    //! Initalizes the tables by trying to contact the initial addresses stored
    //! from previous runs. It will try sending ping requests to these nodes.
    void initializeTable();
    
    //! Performs a table cleanup, usually called by a timer from boost::asio
    //! - removes bad addresses,
    //! - starts bucket refresh,
    //! - sends ping to aging nodes.
    void cleanupTable();

private:
    
    //! Internal mutex to synchronize the various threads
    Mutex mutex;

    //! Address table
    AddressTree _table;

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

    //! Socket 
    udp::socket _socket;
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


}; // dht
}; // torrentsync

BOOST_CLASS_VERSION(torrentsync::dht::RoutingTable, 0);

