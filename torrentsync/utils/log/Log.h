#pragma once

#include <boost/tuple/tuple.hpp>
#include <boost/scoped_ptr.hpp>
#include <ostream>

#include <torrentsync/utils/Lock.h>


namespace torrentsync
{
namespace utils
{
namespace log
{

typedef enum
{
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
} Level;

const std::string& levelToString( const Level );

typedef boost::tuple<
    std::ostream*,
    Level,
    boost::shared_ptr<Mutex> >
        Sink;

} // level
} // utils
} // torrentsync

