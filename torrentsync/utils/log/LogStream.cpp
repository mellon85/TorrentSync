#include <torrentsync/utils/log/LogStream.h>
#include <torrentsync/utils/log/Logger.h>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <mutex>

namespace torrentsync
{
namespace utils
{
namespace log
{

logend_t logend;

static std::mutex log_mutex;

LogStream::LogStream(
    std::list<Sink>& sinks,
    const Level level )
        : _sinks(sinks)
        , _level(level)
        , _buffer(new std::stringstream)
{
    using namespace boost::posix_time;

    ptime timestamp(microsec_clock::universal_time());
    *_buffer << '[' << to_iso_string(timestamp) << ' ' << levelToString(_level) << "] ";
}

void LogStream::flush()
{
    if (Logger::getForceFlush())
    {
        std::for_each( _sinks.begin(), _sinks.end(), [&] (Sink &sink)
        {
            std::get<0>(sink)->flush();
        });
    }
}

template <> LogStream& LogStream::operator<< < logend_t> ( const logend_t& t )
{
    if (_buffer.get())
    {
        *_buffer << std::endl;
        std::for_each( _sinks.begin(), _sinks.end(), [&] (Sink &sink)
        {
            if (std::get<1>(sink) <= _level)
            {
                std::lock_guard<std::mutex> lock(log_mutex);
                *(std::get<0>(sink)) << _buffer->str();
            }
        });
        _buffer.reset();
        flush();
    }
    return *this;
}

} // log
} // utils
} // torrentsync
