#include <string>

#include <torrentsync/utils/log/Log.h>

namespace torrentsync
{
namespace utils
{
namespace log
{

static const std::string levels[] = {
    "ERROR",
    "WARN",
    "INFO",
    "DEBUG" };

const std::string& levelToString( const Level level )
{
    return levels[level];
}

} // level
} // utils
} // torrentsync

