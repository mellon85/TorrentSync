#pragma once

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/optional.hpp>

#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/NodeTree.h>
#include <torrentsync/dht/message/Constants.h>

#include <exception>
#include <mutex>
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
namespace query
{
class Ping;
class FindNode;
};
namespace reply
{
class Ping;
class FindNode;
};
class Query;
class Message;
};

using boost::asio::ip::udp;
using namespace torrentsync;

class RoutingTable
{
public:
    // non copyable
    RoutingTable( const RoutingTable& ) = delete;
    RoutingTable& operator=(const RoutingTable&) = delete;

    // movable
    RoutingTable( RoutingTable&& ) = default;
    RoutingTable& operator=(RoutingTable&&) = default;

    //! Constructor
    //! @param endpoint the port and address to bind
    RoutingTable(
        boost::asio::io_service& io_service);

    virtual ~RoutingTable() = default;

    //! @return DHT table endpoint
    udp::endpoint getEndpoint() const;

    //! Blocking call to look for a node specific node.
    //! Will return a tcp connection to the target node holder.
    //! @TODO should return a class that manages the swarm of nodes, not
    //!       an address. still missing every parameter.
    std::shared_ptr<boost::asio::ip::tcp::socket> lookForNode();

    //! Initializes network sockets binding to the specific endpoint.
    //! May throw exceptions for error
    //! @param endpoint to bind to
    //! @throws boost::system::system_error throw in case of error
    void initializeNetwork(
        const udp::endpoint& endpoint);

    //! returns out node ID
    //! @return the ID
    const NodeData& getNode() const noexcept;

protected:

    //! Sends a message to the specified address
    //! It will send it asynchronously putting them in a queue of at
    //! most MAX_SEND_QUEUE length.
    //! @throws boost::system::system_error
    virtual void sendMessage(
        const utils::Buffer&,
        const udp::endpoint& addr);

    //! @TODO
    virtual void recvMessage(
        const boost::system::error_code& error,
        utils::Buffer buffer,
        std::size_t bytes_transferred,
        const boost::asio::ip::udp::endpoint& sender);

protected:

    //! list of address to populate the table with
    std::list<boost::asio::ip::udp::endpoint> _initial_addresses;

    //! Initalizes the tables by trying to contact the initial addresses stored
    //! from previous runs. It will try sending ping requests to these nodes.
    void initializeTable();

private:

    //! true of false based on the table situation
    std::atomic<bool> _initialization_completed;

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

    //! configure the io_service actions to receive messages
    void scheduleNextReceive();

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
        const utils::Buffer& transactionID,
        const boost::optional<dht::NodeData>& source = boost::optional<dht::NodeData>());

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

    //! Counter of the number of messages to be sent in the queue
    std::atomic<size_t> _send_queue_counter;

    //! Initialization mutex to avoid race condition when serializing
    //! the initializer list.
    mutable std::mutex _initializer_mutex;

    mutable std::mutex _table_mutex;

    //! Callbacks container.
    //! A multimap is enough as anyway there shouldn't be more than one
    //! request at the same time (even though it may happen).
    std::multimap<
        utils::Buffer,
        dht::Callback> _callbacks;

    //! Transaction ID counter.
    std::atomic<uint16_t> _transaction_id;

    //! Returns a new Transacton ID. Starts with a random value and
    //! will increase up to INT16_MAX when it will be reset to 0 and
    //! restart.
    utils::Buffer newTransaction();

    /**
     * Sends an error message to the specified endpoint.
     * It is used to return an error message in case a query can't be
     * fulfilled.
     * @param ip the address to send it to
     * @param transaction the transaction to relate the error to a query
     * @param error the error type
     */
    void sendError(
        const udp::endpoint& ip,
        const utils::Buffer& transaction,
        const dht::message::ErrorType::error_type error);

    //! ************** Message handlers *****************

    //! Handle ping queries.
    void handlePingQuery(
        const dht::message::query::Ping&,
        const dht::Node&);

    /** Handle ping reply
     * nothing to do in this case.
     * The default behaviour is to try to add the 
     * nome to the known nodes or to mark it as a good node.
     */
    void handlePingReply(
        const dht::message::reply::Ping&,
        const dht::Node&);

    //! Handle find_node queries.
    void handleFindNodeQuery(
        const dht::message::query::FindNode&,
        const dht::Node&);

    /** Handle find_node replies
     * This handler should never be called. find_node queries should always 
     * belong to a use case not randomly received.
     */
    void handleFindNodeReply(
        const dht::message::reply::FindNode&,
        const dht::Node&);

    //! sends a ping message to the destination node (and setup a callback to receive).
    void doPing( dht::Node& destination );
};

template <class Archive>
void RoutingTable::save( Archive &ar, const unsigned int version) const
{
    if (version <= 0)
    {
        std::lock_guard<std::mutex> lock(_initializer_mutex);
        std::lock_guard<std::mutex> lock_table(_table_mutex);

        ar << _table.size();
        // serialize addresses in _table
        /*for ( auto address : _table )
        {
            ar << address;
        };*/

        // serialize addresses in _initial_addresses
        for( auto e : _initial_addresses )
        {
           ar << e;
        };

        throw std::runtime_error("Not Implemented Yet");
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

