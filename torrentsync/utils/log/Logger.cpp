#include <torrentsync/utils/log/Logger.h>

namespace torrentsync
{
namespace utils
{
namespace log
{

bool Logger::_forceFlush = false;

Level Logger::_level = WARN;

boost::scoped_ptr<Logger> Logger::_logger;

Logger::Logger()
{
}

Logger& Logger::getInstance()
{
    if (!_logger.get())
    {
        _logger.reset(new Logger());
    }
    return *_logger;
}

LogStream Logger::log( const Level level )
{
    return LogStream(_sinks,level);
}

void Logger::setForceFlush( const bool forseFlush )
{
    _forceFlush = forseFlush;
}

bool Logger::getForceFlush()
{
    return _forceFlush;
}

void Logger::setLogLevel( const Level level )
{
    _level = level;
}

Level Logger::getLogLevel()
{
    return _level;
}

void Logger::addSink( std::ostream* stream, const Level level )
{
    _sinks.push_back(Sink(stream,level,
        boost::shared_ptr<Mutex>(new Mutex())));
}

void Logger::destroy()
{
    _logger.reset();
}

} // log
} // utils
} // torrentsync
