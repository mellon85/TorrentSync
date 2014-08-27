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
    DEBUG = 0,
    INFO,
    WARN,
    ERROR,
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

