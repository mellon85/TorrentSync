#pragma once

#include <torrentsync/utils/log/Log.h>
#include <torrentsync/utils/log/LogStream.h>

#include <list>

//#include <boost/utility.hpp>

#define LOG(LEVEL, MSG)                                                        \
  {                                                                            \
    auto &_logger = torrentsync::utils::log::Logger::getInstance();            \
    auto _level = torrentsync::utils::log::LEVEL;                              \
    if (_logger.willLog(_level)) {                                             \
      _logger.log(_level) << MSG << torrentsync::utils::log::logend;           \
    }                                                                          \
  }

namespace torrentsync {
namespace utils {
namespace log {

/**
 * Default log level: WARN
 **/
class Logger : boost::noncopyable {
public:
  static Logger &getInstance() noexcept;

  LogStream log(const Level level);

  //! Sets the policy to force the log to be flushed at each operations
  //! @param forceFlush boolean value for the flag
  static void setForceFlush(const bool forceFlush);

  //! Returns the value of the global flag to force log files to be flushed
  //! @return the flag value
  static bool getForceFlush() noexcept;

  static Level getLogLevel() noexcept;

  // Adding sinks in not thread safe. No other logging action should happend
  // at the same time.
  void addSink(std::unique_ptr<std::ostream> &&, const Level);

  // tests if the log level will be logged
  bool willLog(const Level) const noexcept;

  // It's not thread safe, should be called only when all logging has
  // completed.
  void destroy();

private:
  //! Logger constructor
  Logger();

  static Level _level;

  //! this flag determines if the logs must be flushed after any message is !
  //! sent. Very useful in case of crashes that happends when the log is still
  //! buffered.. Enable only when necessary, by default it's false.
  static bool _forceFlush;

  //! Registered sinks to store the logs to
  std::list<Sink> _sinks;
};

} // log
} // utils
} // torrentsync
