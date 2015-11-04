#pragma once

#include <tuple>
#include <ostream>
#include <memory>

namespace torrentsync {
namespace utils {
namespace log {

typedef enum { DATA = 0, DEBUG = 1, INFO = 2, WARN = 3, ERROR = 4 } Level;

const std::string &levelToString(const Level);

typedef std::tuple<std::unique_ptr<std::ostream>, Level> Sink;

} // level
} // utils
} // torrentsync
