#pragma once

#include <torrentsync/utils/log/Log.h>

#include <assert.h>
#include <boost/foreach.hpp>
#include <list>
#include <sstream>

namespace torrentsync {
namespace utils {
namespace log {

class logend_t {};
extern logend_t logend;

class LogStream {
public:
  LogStream(std::list<Sink> &sinks, const Level level);

  ~LogStream() = default;

  LogStream(LogStream &&) = default;
  LogStream &operator=(LogStream &&) = default;

  LogStream(const LogStream &) = delete;
  LogStream &operator=(const LogStream &) = delete;

  template <class T> LogStream &operator<<(const T &t);
  LogStream &operator<<(const logend_t &t);

private:
  std::list<Sink> &_sinks;

  Level _level;

  void flush();

  std::unique_ptr<std::stringstream> _buffer;
};

template <class T> LogStream &LogStream::operator<<(const T &t) {
  *_buffer << t;
  return *this;
}

} // log
} // utils
} // torrentsync
