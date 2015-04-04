#pragma once

#include <tuple>
#include <ostream>
#include <mutex>
#include <memory>

namespace torrentsync
{
namespace utils
{
namespace log
{

typedef enum
{
    DATA  = 0,
    DEBUG = 1,
    INFO  = 2,
    WARN  = 3,
    ERROR = 4
} Level;

const std::string& levelToString( const Level );

typedef std::tuple<
    std::ostream*,
    Level,
    std::shared_ptr<std::mutex> >
        Sink;

} // level
} // utils
} // torrentsync

