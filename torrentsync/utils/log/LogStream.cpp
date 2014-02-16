#include <torrentsync/utils/log/LogStream.h>
#include <torrentsync/utils/log/Logger.h>

#include <boost/foreach.hpp>
#include "boost/date_time/posix_time/posix_time.hpp"

namespace torrentsync
{
namespace utils
{
namespace log
{

const logend_t logend;

LogStream::LogStream(
    std::vector<Sink>& sinks,
    const Level level )
        : _sinks(sinks)
        , _level(level)
        , _buffer(new std::stringstream)
{
    using namespace boost::posix_time;

    if (_level < Logger::getLogLevel())
    {
        _level = Logger::getLogLevel();
    }

    ptime timestamp(microsec_clock::universal_time());
    *_buffer << '[' << to_iso_string(timestamp) << ' ' << levelToString(_level) << "] ";
}

LogStream::~LogStream()
{
}

void LogStream::flush()
{
    if (Logger::getForceFlush())
    {
        BOOST_FOREACH( Sink &sink, _sinks )
        {
            sink.get<0>()->flush();
        }
    }
}

template <> LogStream& LogStream::operator<< < logend_t> ( const logend_t& t )
{
    if (_buffer.get())
    {
        *_buffer << std::endl;
        BOOST_FOREACH( Sink &sink, _sinks )
        {
            if (sink.get<1>() <= _level)
            {
                WriteLock lock(*sink.get<2>());
                (*(sink.get<0>())) << _buffer->str();
            }
        }
        _buffer.reset();
        flush();
    }
}

} // log
} // utils
} // torrentsync
