#pragma once

#include <torrentsync/dht/RoutingTable.h>
#include <boost/asio.hpp>

namespace torrentsync {

/** Represents the application itself.
    Initializes and connects the various parts of the library.
 */
class App {
public:
  //! @TODO Loads the configuration
  App(/* configuration */);

  //! this call will not return until the application has to stop.
  void runloop();

private:
  //! Main io_service
  boost::asio::io_service _service;

  //! Work system to avoid closing the run() call to the service
  //! without calling stop
  boost::asio::io_service::work _work;

  //! Signal mask for the signal to check
  boost::asio::signal_set _stop_signal;

  //! Setup up signal handlers
  void setupSignalHandlers();

  //! The DHT routing table
  torrentsync::dht::RoutingTable _table;
};

}; // torrentsync
