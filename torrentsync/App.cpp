#include <torrentsync/App.h> 
#include <boost/asio.hpp>
#include <torrentsync/utils/log/Logger.h>

namespace torrentsync
{

App::App( /* configuration */ ) :
    _service(),
    _work(_service),
    _stop_signal(_service, SIGINT, SIGTERM),
    _table(boost::asio::ip::udp::endpoint(),_service)
{
    // Initializes the various part of the application
    setupSignalHandlers();
}

void App::runloop()
{
    // runs the main threads and all the io_services
    LOG(DEBUG,"App * Runloop started");
    _service.run();
    LOG(DEBUG,"App * Runloop ended");
}

void App::setupSignalHandlers()
{
    _stop_signal.async_wait([&](
        const boost::system::error_code& error,
        int signal_number) -> void{
            if (!error)
            {
                LOG(INFO,"Received signal: " << signal_number);
                _service.stop();
            }
            else
            {
                LOG(ERROR, "App * Error in signal handler: " <<
                    error.message() << " (" << error << ")" );
            }
        });
    LOG(DEBUG,"App * Signals configured");
}

void App::setupIOServices()
{
}

}; // torrentsync
