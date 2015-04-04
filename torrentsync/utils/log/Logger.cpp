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

std::unique_ptr<Logger> Logger::_logger;

Logger::Logger()
{
}

Logger& Logger::getInstance()
{
    static std::mutex mutex;
    std::lock_guard<std::mutex> lock(mutex);

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

bool Logger::willLog( const Level level ) const noexcept
{
    return _level <= level;
}

void Logger::setForceFlush( const bool forseFlush )
{
    _forceFlush = forseFlush;
}

bool Logger::getForceFlush()
{
    return _forceFlush;
}

Level Logger::getLogLevel()
{
    return _level;
}

void Logger::addSink( std::ostream* stream, const Level level )
{
    if ( level < _level )
        _level = level;

    _sinks.push_back(Sink(stream,level,
        std::shared_ptr<std::mutex>(new std::mutex())));
}

void Logger::destroy()
{
    _logger.reset();
}

} // log
} // utils
} // torrentsync
