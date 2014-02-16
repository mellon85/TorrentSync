#pragma once

#include <torrentsync/utils/log/Log.h>
#include <torrentsync/utils/log/LogStream.h>

#include <boost/utility.hpp>
#include <boost/scoped_ptr.hpp>

#define LOG(LEVEL, MSG) torrentsync::utils::log::Logger::getInstance().log(LEVEL) << MSG << \
    torrentsync::utils::log::logend;

namespace torrentsync
{
namespace utils
{
namespace log
{

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

    static void setLogLevel( const Level level );

    static Level getLogLevel();

    void addSink( std::ostream*, const Level );

    static void destroy();

private:
    //! Logger constructor
    Logger();

    //! the logger singleton instance
    static boost::scoped_ptr<Logger> _logger;

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
