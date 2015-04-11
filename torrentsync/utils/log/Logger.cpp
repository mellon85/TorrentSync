#include <torrentsync/utils/log/Logger.h>
#include <mutex>

namespace torrentsync
{
namespace utils
{
namespace log
{

bool Logger::_forceFlush = false;

Level Logger::_level = WARN;

Logger::Logger()
{
}

Logger& Logger::getInstance() noexcept
{
    //! the logger singleton instance
    static Logger _logger;

    return _logger;
}

LogStream Logger::log( const Level level )
{
    return LogStream(_sinks,level);
}

bool Logger::willLog( const Level level ) const noexcept
{
    return _level <= level;
}

void Logger::setForceFlush( const bool forseFlush )
{
    _forceFlush = forseFlush;
}

bool Logger::getForceFlush() noexcept
{
    return _forceFlush;
}

Level Logger::getLogLevel() noexcept
{
    return _level;
}

void Logger::destroy()
{
    auto&& logger = getInstance();

    std::list<Sink> old_sinks;
    logger._sinks.swap(old_sinks);
}

void Logger::addSink( std::unique_ptr<std::ostream>&& stream, const Level level )
{
    if ( level < _level )
        _level = level;

    _sinks.push_back(
        Sink(
            std::move(stream),
            level,
            std::move(std::unique_ptr<std::mutex>(new std::mutex()))));
}

} // log
} // utils
} // torrentsync
