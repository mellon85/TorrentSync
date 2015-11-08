#include <boost/regex.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/log/LogStream.h>

BOOST_AUTO_TEST_SUITE(torrentsync_utils_log);

using namespace torrentsync::utils::log;

BOOST_AUTO_TEST_CASE(initialize_and_close) { Logger::getInstance(); }

BOOST_AUTO_TEST_CASE(log_message) {
  std::stringstream *s_ptr = new std::stringstream();
  std::unique_ptr<std::stringstream> str(s_ptr);

  std::istream result(str->rdbuf());
  Logger::getInstance().addSink(std::move(str), DEBUG);
  Logger::getInstance().log(DEBUG) << "message"
                                   << " plpl" << logend;
  boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{1,6} \\w+\\] message plpl.{1,2}",
                     boost::regex::perl);
  BOOST_REQUIRE(regex_match(s_ptr->str(), regex));
  Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_message2) {
  std::stringstream *s_ptr = new std::stringstream();
  std::unique_ptr<std::stringstream> str(s_ptr);
  Logger::getInstance().addSink(std::move(str), DEBUG);
  LOG(DEBUG, "message"
                 << " plpl");
  boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] message plpl.{1,2}",
                     boost::regex::perl);
  BOOST_REQUIRE(regex_match(s_ptr->str(), regex));
  Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_multipleSink) {
  std::stringstream *s_ptr = new std::stringstream();
  std::unique_ptr<std::stringstream> str(s_ptr);
  std::stringstream *s_ptr2 = new std::stringstream();
  std::unique_ptr<std::stringstream> str2(s_ptr2);

  Logger::getInstance().addSink(std::move(str), DEBUG);
  Logger::getInstance().addSink(std::move(str2), DEBUG);

  LOG(DEBUG, "message"
                 << " plpl");

  boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] message plpl.{1,2}",
                     boost::regex::perl);
  BOOST_REQUIRE(regex_match(s_ptr->str(), regex));
  BOOST_REQUIRE(regex_match(s_ptr2->str(), regex));
  Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_moreMessages) {
  std::stringstream *s_ptr = new std::stringstream();
  std::unique_ptr<std::stringstream> str(s_ptr);
  Logger::getInstance().addSink(std::move(str), DEBUG);

  LOG(DEBUG, "msg1");
  LOG(DEBUG, "msg2");

  boost::regex regex1("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] "
                      "msg1.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] msg2.{1,2}",
                      boost::regex::perl);
  BOOST_REQUIRE(regex_match(s_ptr->str(), regex1));
  Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_forceFlush) {
  std::stringstream *s_ptr = new std::stringstream();
  std::unique_ptr<std::stringstream> str(s_ptr);
  std::stringstream *s_ptr2 = new std::stringstream();
  std::unique_ptr<std::stringstream> str2(s_ptr2);

  Logger::getInstance().addSink(std::move(str), DEBUG);
  Logger::getInstance().addSink(std::move(str2), DEBUG);

  Logger::setForceFlush(true);

  std::string output;

  LOG(DEBUG, "msg1");
  LOG(DEBUG, "msg2");

  boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] "
                     "msg1.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] msg2.{1,2}",
                     boost::regex::perl);
  BOOST_REQUIRE(regex_match(s_ptr->str(), regex));
  BOOST_REQUIRE(regex_match(s_ptr2->str(), regex));

  Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_levels) {
  std::stringstream *s_ptr = new std::stringstream();
  std::unique_ptr<std::stringstream> str(s_ptr);
  std::stringstream *s_ptr2 = new std::stringstream();
  std::unique_ptr<std::stringstream> str2(s_ptr2);
  std::stringstream *s_ptr3 = new std::stringstream();
  std::unique_ptr<std::stringstream> str3(s_ptr3);

  Logger::getInstance().addSink(std::move(str), DEBUG);
  Logger::getInstance().addSink(std::move(str2), WARN);
  Logger::getInstance().addSink(std::move(str3), ERROR);

  LOG(DEBUG, "debug");
  LOG(WARN, "warn");
  LOG(ERROR, "error");

  boost::regex regex_3(
      "\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] debug.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} "
      "\\w+\\] warn.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] error.{1,2}",
      boost::regex::perl);
  boost::regex regex_2("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] "
                       "warn.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] "
                       "error.{1,2}",
                       boost::regex::perl);
  boost::regex regex_1("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] error.{1,2}",
                       boost::regex::perl);

  BOOST_CHECK(regex_match(s_ptr->str(), regex_3));
  BOOST_CHECK(regex_match(s_ptr2->str(), regex_2));
  BOOST_CHECK(regex_match(s_ptr3->str(), regex_1));
  Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_SUITE_END();
