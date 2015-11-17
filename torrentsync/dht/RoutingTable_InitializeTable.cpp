#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/RoutingTable.h>

#include <iterator>
#include <array>
#include <atomic>
#include <mutex>
#include <tuple>

#include <boost/asio.hpp>

//! list of known bootstrap servers for the DHT network
static const std::array< // domain/ip address, port, needs to be resolved
    std::pair<const char *, const char *>, 3> BOOTSTRAP_ADDRESSES = {
    std::make_pair("router.bittorrent.com", "8991"),
    std::make_pair("router.utorrent.com", "6881"),
    std::make_pair("dht.transmissionbt.com", "6881")};

//! Number of nodes that return my address when looking for my node
//! to be sure that I can't get any closer.
static const size_t DHT_CLOSE_ENOUGH = 10;

//! number of batches per second while initializing the DHT.
static const size_t INITIALIZE_PING_BATCH_INTERVAL = 1000;

namespace torrentsync {
namespace dht {

namespace msg = dht::message;
using namespace torrentsync;

void RoutingTable::initializeTable() {
  if (_initialization_completed)
    return;

  auto timer = std::make_shared<boost::asio::deadline_timer>(
      _io_service,
      boost::posix_time::milliseconds(INITIALIZE_PING_BATCH_INTERVAL));

  LOG(DEBUG, "RoutingTable * Register initializeTable timer");
  timer->async_wait([&, timer](const boost::system::error_code &e) {
    using namespace boost::asio;
    initializeTable();

    if (e.value() != 0) {
      LOG(ERROR,
          "Error in RoutingTable initializeTable timer: " << e.message());
      return;
    }

    // still running from previous execution! skip this round
    // otherwise get the lock and go on
    std::unique_lock<std::mutex> lock_table(_initializer_mutex, std::try_to_lock);
    if (!lock_table.owns_lock())
    {
        LOG(DEBUG, "RoutingTable * Initializations till running");
        return;
    }

    LOG(DEBUG, "RoutingTable * " << _initial_addresses.size()
                                 << " initializing addresses");


    if (_initial_addresses.empty()) {
      bootstrap();
      LOG(DEBUG, "RoutingTable * "
                     << _initial_addresses.size()
                     << " initializing addresses after bootstrap");
    }

    for (size_t i = 0;
         i < INITIALIZE_PING_BATCH_SIZE && !_initial_addresses.empty(); ++i) {
      // copy to local and remove from list
      const auto endpoint = _initial_addresses.front();
      _initial_addresses.pop_front();

      LOG(DEBUG, "RoutingTable * initializing ping with " << endpoint);

      const auto transaction = newTransaction();

      // create and send the message
      const utils::Buffer msg = dht::message::query::FindNode::make(
          transaction, _table.getTableNode(), _table.getTableNode().write());

      registerCallback(
          [&](const boost::optional<Callback::payload_type> data,
              const dht::Callback &) {

            if (!data)
              return;

            // there is no assert for no-exception
            const auto *reply = boost::get<msg::reply::Reply>(&data->message);
            assert(reply != nullptr);

            const auto *error = boost::get<msg::reply::Error>(reply);
            if (error != nullptr) {
              LOG(DEBUG, "Error returned in initialization: " << error);
              return;
            }

            const auto *find_node = boost::get<msg::reply::FindNode>(reply);
            if (find_node == nullptr) {
              LOG(WARN, "A message different from find node received");
              return;
            }

            for (const auto &t : find_node->getNodes()) {
              if (*t == _table.getTableNode()) {
                _initialization_completed = true;
                LOG(INFO, " Initialization finished");
                break;
              } else {
                if (!!t->getEndpoint())
                  _initial_addresses.push_front(*t->getEndpoint());
                LOG(DEBUG, "Distance: " << (*t ^ _table.getTableNode()));
                std::lock_guard<std::mutex> lock_table(_table_mutex);
                _table.addNode(t);
              }
            }
          },
          transaction);

      // send the message
      sendMessage(msg, endpoint);
    }

    LOG(DEBUG, "RoutingTable * " << _initial_addresses.size()
                                 << " initializing addresses remaining");

  });
}

void RoutingTable::bootstrap() {
  if (!_initial_addresses.empty()) {
    LOG(INFO, "RoutingTable * bootstrap Can't bootstrap while I still "
                  << "have initial addresses");
    return;
  }

  std::lock_guard<std::mutex> lock_table(_table_mutex);
  LOG(INFO, "RoutingTable * Proceeding with boostrap procedure from "
                << "known nodes; count: " << _table.size());

  udp::resolver resolver(_io_service);

  for (const auto &addr : BOOTSTRAP_ADDRESSES) {
    LOG(DEBUG, "Bootstrapping with " << addr.first << ":" << addr.second);
    boost::system::error_code error;
    udp::resolver::query query(udp::v4(), addr.first, addr.second);
    udp::resolver::iterator iterator = resolver.resolve(query, error);

    while (iterator != udp::resolver::iterator()) {
      LOG(DEBUG, "Resolved to " << iterator->endpoint());
      _initial_addresses.push_back(iterator->endpoint());
      ++iterator;
    }
  }
}

}; // dht
}; // torrentsync
