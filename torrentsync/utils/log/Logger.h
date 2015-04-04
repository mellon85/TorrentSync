#pragma once

#include <torrentsync/utils/log/Log.h>
#include <torrentsync/utils/log/LogStream.h>

#include <mutex>

//#include <boost/utility.hpp>

#define LOG(LEVEL, MSG) {\
auto& _logger = torrentsync::utils::log::Logger::getInstance();\
auto _level = torrentsync::utils::log::LEVEL;\
if( _logger.willLog(_level) ) { _logger.log(_level) << MSG <<\
    torrentsync::utils::log::logend;}}

namespace torrentsync
{
namespace utils
{
namespace log
{

/**
 * Default log level: WARN
 **/
class Logger : boost::noncopyable
{
public:

    static Logger& getInstance();
    
    LogStream log( const Level level );

    //! Sets the policy to force the log to be flushed at each operations
    //! @param forceFlush boolean value for the flag
    static void setForceFlush( const bool forceFlush );

    //! Returns the value of the global flag to force log files to be flushed
    //! @return the flag value
    static bool getForceFlush();

    static Level getLogLevel();

    void addSink( std::ostream*, const Level );

    static void destroy();

    bool willLog( const Level ) const noexcept;

private:
    //! Logger constructor
    Logger();

    //! the logger singleton instance
    static std::unique_ptr<Logger> _logger;

    static Level _level;

    //! this flag determines if the logs must be flushed after any message is !
    //! sent. Very useful in case of crashes that happends when the log is still
    //! buffered.. Enable only when necessary, by default it's false.
    static bool _forceFlush;

    //! Registered sinks to store the logs to
    std::vector<Sink> _sinks;

};

} // log
} // utils
} // torrentsync
