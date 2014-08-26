#pragma once

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/optional.hpp>

#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/NodeTree.h>
#include <torrentsync/utils/Lock.h>

#include <exception>
#include <map>
#include <list>
#include <utility>
#include <atomic>

namespace torrentsync
{
namespace dht
{

//! Number of addresses per batch while initialing the DHT, should be configurable.
static const size_t INITIALIZE_PING_BATCH_SIZE = 5;

namespace message
{
class Ping;
class FindNode;
class Message;
};

using boost::asio::ip::udp;
using namespace torrentsync;

class RoutingTable : public boost::noncopyable
{
public:
    //! Constructor
    //! @param endpoint the port and address to bind
    RoutingTable(
        boost::asio::io_service& io_service);

    virtual ~RoutingTable();

    //! @return DHT table endpoint
    udp::endpoint getEndpoint() const;

    //! Blocking call to look for a node specific node.
    //! Will return a tcp connection to the target node holder.
    //! @TODO should return a class that manages the swarm of nodes, not
    //!       an address. still missing every parameter.
    boost::shared_ptr<boost::asio::ip::tcp::socket> lookForNode();

    //! Initializes network sockets binding to the specific endpoint.
    //! May throw exceptions for error
    //! @param endpoint to bind to
    //! @throws boost::system::system_error throw in case of error
    void initializeNetwork(
        const udp::endpoint& endpoint);

protected:
    //! Shared pointer containing a timeout timer
    typedef boost::shared_ptr<boost::asio::deadline_timer> shared_timer;

    //! Initalizes the tables by trying to contact the initial addresses stored
    //! from previous runs. It will try sending ping requests to these nodes.
    void initializeTable( shared_timer timer = shared_timer());
    
    //! Use a few known addresses to start a connection with the DHT network.
    //! This function must not be called until initialization of the
    //! table has not finished.
    void bootstrap();

    //! Performs a table cleanup, usually called by a timer from boost::asio
    //! - removes bad addresses,
    //! - starts bucket refresh,
    //! - sends ping to aging nodes.
    //! - look for close nodes.
    //! - clean timedout callbacks.
    void tableMaintenance();

    //! Sends a message to the specified address
    //! It will send it asynchronously putting them in a queue of at
    //! most MAX_SEND_QUEUE length.
    //! @throws boost::system::system_error
    virtual void sendMessage(
        const utils::Buffer&,
        const udp::endpoint& addr);

    //! @TODO
    void recvMessage(
        const boost::system::error_code& error,
        utils::Buffer buffer,
        std::size_t bytes_transferred,
        const boost::asio::ip::udp::endpoint& sender);

    //! list of address to populate the table with
    std::list<boost::asio::ip::udp::endpoint> _initial_addresses;

    //! configure the io_service actions to receive messages
    void scheduleNextReceive();

    //! sends a ping message to the destination node (and setup a callback to receive).
    void doPing( dht::Node& destination );

private:

    //! returns the most specific callback, and will be removed from the
    //! callbacks.
    boost::optional<Callback> getCallback(
        const message::Message& message);

    //! Registers a callback to be called when we receive a message.
    //! It will be executed only once and before any other processing.
    //! The callback will be removed in any case if it is not used in less than 60 seconds.
    //! @param func is the function to call
    //! @param type mandatory parameter specifying the type of the message to signal.
    //!             must be from dht::messages::Messages.
    //! @param source optional parameter specifing if the message is awaited from a specific Peer
    //! @param transactionID optional parameter specifing if the message is awaited from a specific Peer
    void registerCallback(
        const Callback::callback_t& func,
        const std::string& type,
        const std::string& messageType,
        const dht::NodeData& source, 
        const utils::Buffer& transactionID);

    //! Node table
    NodeTree _table;

    //! Serialization friend class
    friend class boost::serialization::access;
    
    //! Split the serialization in 2 different steps
    BOOST_SERIALIZATION_SPLIT_MEMBER();

    template <class Archive>
    void save( Archive &ar, const unsigned int version) const;

    //! Loading serialize structure
    //! @throws std::invalid_argument in case the file is not readable,
    //!         damaged of an unsupported version.
    template <class Archive>
    void load( Archive &ar, const unsigned int version);

    //! IO service of for the routing table
    boost::asio::io_service& _io_service;

    //! Inbound socket 
    udp::socket _recv_socket;

    //! Outbound socket 
    udp::socket _send_socket;

    //! Outbout mutex
    Mutex _send_mutex;

    //! Callbacks container.
    //! A multimap is enough as anyway there shouldn't be more than one
    //! request at the same time (even though it may happen).
    std::multimap<
        dht::NodeData,
        dht::Callback> _callbacks;

    //! Number of close nodes found.
    std::atomic<size_t> _close_nodes_count;

    //! ************** Message handlers *****************

    //! Handle ping queries.
    void handlePingQuery(
        const dht::message::Ping&,
        const dht::Node&);

    /** Handle ping reply
     * nothing to do in this case.
     * The default behaviour is to try to add the 
     * nome to the known nodes or to mark it as a good node.
     */
    void handlePingReply(
        const dht::message::Ping&,
        const dht::Node&);

    //! Handle find_node queries.
    void handleFindNodeQuery(
        const dht::message::FindNode&,
        const dht::Node&);

    /** Handle find_node replies
     * This handler should never be called. find_node queries should always 
     * belong to a use case not randomly received.
     */
    void handleFindNodeReply(
        const dht::message::FindNode&,
        const dht::Node&);

};

template <class Archive>
void RoutingTable::save( Archive &ar, const unsigned int version) const
{
    if (version <= 0)
    {
        ar << _table.size();
        throw std::runtime_error("Not Implemented Yet");
        // @TODO
    }
}

template <class Archive>
void RoutingTable::load( Archive &ar, const unsigned int version)
{
    if (version > 0 )
    {
        throw std::invalid_argument("Unsupported file version");
    }
    size_t nodes_count;
    ar >> nodes_count;
    throw std::runtime_error("Not Implemented Yet");
    // @TODO 
    // refresh all the nodes
    // 1. ping all known and insert them in the routing table with the normal procedure
    // 2. perform normal startup operation and let the bucket refreshing do it's job
}

}; // dht
}; // torrentsync

BOOST_CLASS_VERSION(torrentsync::dht::RoutingTable, 0);

