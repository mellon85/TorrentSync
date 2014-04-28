#pragma once

#include <torrentsync/utils/log/Log.h>

#include <assert.h>
#include <boost/foreach.hpp>
#include <vector>
#include <sstream>

namespace torrentsync
{
namespace utils
{
namespace log
{

class logend_t {};
extern logend_t logend;

class LogStream
{
public:
    LogStream( std::vector<Sink> & sinks, const Level level );

    ~LogStream();

    template <class T> LogStream& operator<< ( const T& t );

private:

    std::vector<Sink> & _sinks;

    Level _level;

    void flush();

    boost::shared_ptr<std::stringstream> _buffer;
};

template <> LogStream& LogStream::operator<< < logend_t> ( const logend_t& t );
template <class T> LogStream& LogStream::operator<< ( const T& t )
{
    *_buffer << t;
    return *this;
}

} // log
} // utils
} // torrentsync
