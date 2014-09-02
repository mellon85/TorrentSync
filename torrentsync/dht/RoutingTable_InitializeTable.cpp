#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/message/reply/FindNode.h>

#include <iterator>
#include <vector>
#include <atomic>
#include <mutex>
#include <tuple>

#include <boost/asio.hpp>


//! list of known bootstrap servers for the DHT network
static std::vector< // domain/ip address, port, needs to be resolved
    std::pair<std::string,std::string> > BOOTSTRAP_ADDRESSES =
            {std::make_pair("router.bittorrent.com"     ,"8991"),
             std::make_pair("router.utorrent.com"       ,"6881"),
             std::make_pair("dht.transmissionbt.com"    ,"6881")
    };

//! Number of nodes that return my address when looking for my node
//! to be sure that I can't get any closer.
static const size_t DHT_CLOSE_ENOUGH = 10;

//! number of batches per second while initializing the DHT.
static const size_t INITIALIZE_PING_BATCH_INTERVAL = static_cast<size_t>(3000);

namespace torrentsync
{
namespace dht
{

namespace msg = dht::message;
using namespace torrentsync;

void RoutingTable::initializeTable()
{
    using timer_t = boost::asio::deadline_timer;
     std::shared_ptr<timer_t> timer(new timer_t(_io_service,
        boost::posix_time::milliseconds(INITIALIZE_PING_BATCH_INTERVAL)));

    LOG(DEBUG, "RoutingTable * Register initializeTable timer");
    timer->async_wait(
        [&,timer] (const boost::system::error_code& e) {
                using namespace boost::asio;

                LOG(DEBUG, "RoutingTable * " << _initial_addresses.size() <<
                                " initializing addresses");

                if ( e.value() != 0 )
                {
                    LOG(ERROR, "Error in RoutingTable initializeTable timer: " << e.message());
                    initializeTable();
                    return;
                }

                if ( _initial_addresses.empty() )
                {
                    bootstrap();
                    LOG(DEBUG, "RoutingTable * " << _initial_addresses.size() <<
                                    " initializing addresses after bootstrap");
                }

                for( size_t i = 0; 
                    i < INITIALIZE_PING_BATCH_SIZE && !_initial_addresses.empty(); ++i )
                {   
                    // copy to local and remove from list
                    const auto endpoint = _initial_addresses.front();
                    _initial_addresses.pop_front();

                    LOG(DEBUG, "RoutingTable * initializing ping with " << endpoint);

                    const auto transaction = newTransaction();
                    
                    // create and send the message
                    const utils::Buffer msg =
                        dht::message::query::FindNode::make(
                            transaction,
                            _table.getTableNode(),
                            _table.getTableNode());
                    
                    registerCallback([&](
                        boost::optional<Callback::payload_type> data,
                        const dht::Callback& trigger) {

                        if (!!data)
                        {
                            try
                            {
                                const msg::reply::FindNode& find_node = dynamic_cast<const msg::reply::FindNode&>(data->message);
                                auto nodes = find_node.getNodes();
                                std::for_each( nodes.begin(), nodes.end(),
                                    [&]( const dht::NodeSPtr& t )
                                {
                                    if (!!t->getEndpoint())
                                        _initial_addresses.push_front(*t->getEndpoint());
                                    _table.addNode(NodeSPtr(t));
                                });
                            }
                            catch(  std::bad_cast& e )
                            {
                                LOG(WARN, "A message different from find node received");
                                return;
                            }
                            // put all the nodes received at the front of _initial_addresses
                            // _close_nodes_count must be incremented as necessary
                        }
                    }, transaction);

                    // send the message
                    sendMessage(msg,endpoint);
                }

                LOG(DEBUG, "RoutingTable * " << _initial_addresses.size() <<
                                " initializing addresses remaining");

                if ( _close_nodes_count < DHT_CLOSE_ENOUGH )
                    initializeTable();
                else
                    _initial_addresses.clear();
            });
}

void RoutingTable::bootstrap()
{
    if (!_initial_addresses.empty())
    {
        LOG(INFO,"RoutingTable * bootstrap Can't bootstrap while I still " <<
                "have initial addresses");
        return;
    }
    
    LOG(INFO,"RoutingTable * Proceeding with boostrap procedure from " <<
            "known nodes; count: " << _table.size() << ", needed: " << _close_nodes_count);

    udp::resolver resolver(_io_service);

    std::for_each( BOOTSTRAP_ADDRESSES.begin(), BOOTSTRAP_ADDRESSES.end(),
        [&](const std::pair<std::string,std::string>& addr)
    {
        LOG(DEBUG,"Bootstrapping with "<<addr.first<<":"<<addr.second);
        boost::system::error_code error;
        udp::resolver::query query(udp::v4(), addr.first, addr.second);
        udp::resolver::iterator iterator = resolver.resolve(query,error);

        while ( iterator != udp::resolver::iterator() )
        {
            LOG(DEBUG,"Resolved to "<< iterator->host_name() );
            _initial_addresses.push_back(iterator->endpoint());
            ++iterator;
        }
    });
}

}; // dht
}; // torrentsync
