#pragma once

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/optional.hpp>

#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/NodeTree.h>
#include <torrentsync/dht/Peer.h>
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

    virtual ~RoutingTable();

    const udp::endpoint getEndpoint() const;

    boost::asio::io_service& getIO_service();

protected:
    typedef boost::shared_ptr<boost::asio::deadline_timer> shared_timer;

    //! Initalizes the tables by trying to contact the initial addresses stored
    //! from previous runs. It will try sending ping requests to these nodes.
    void initializeTable( shared_timer timer = shared_timer());
    
    //! Performs a table cleanup, usually called by a timer from boost::asio
    //! - removes bad addresses,
    //! - starts bucket refresh,
    //! - sends ping to aging nodes.
    //! - look for close nodes.
    void tableMaintenance();

    //! TODO
    virtual void sendMessage();

    //! TODO
    virtual void recvMessage();

    //! Initializes network sockets binding to the specific endpoint.
    //! May throw exceptions for error
    //! @param endpoint to bind to
    //! @throws boost::system::system_error throw in case of error
    virtual void initializeNetwork(
        const udp::endpoint& endpoint );
    
    //! list of address to populate the table with
    std::list<Peer> _initial_addresses;

private:

    //! Registers a callback to be called when we receive a message.
    //! It will be executed only once and before any other processing.
    //! The callback will be removed in any case if it is not used in less than 60 seconds.
    //! @param func is the function to call
    //! @param type mandatory parameter specifying the type of the message to signal.
    //!             must be from dht::messages::Messages.
    //! @param source optional parameter specifing if the message is awaited from a specific Peer
    //! @param transactionID optional parameter specifing if the message is awaited from a specific Peer
    void registerCallback(
        const std::function<void (const torrentsync::dht::message::Message&)>& func,
        const std::string& type,
        const boost::optional<torrentsync::dht::Peer>& source, 
        const boost::optional<torrentsync::utils::Buffer>& transactionID);

    //! Internal mutex to synchronize the various threads
    mutable Mutex mutex;

    //! Node table
    NodeTree _table;

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

    //! Callbacks list
    std::list< Callback > _callbacks;
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

