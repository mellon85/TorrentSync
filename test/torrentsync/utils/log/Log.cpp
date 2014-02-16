#include <boost/regex.hpp>
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/log/LogStream.h>

BOOST_AUTO_TEST_SUITE(torrentsync_utils_log);

using namespace torrentsync::utils::log;

BOOST_AUTO_TEST_CASE(initialize_and_close)
{
    Logger::getInstance();
}

BOOST_AUTO_TEST_CASE(log_message)
{
    std::stringstream str;
    Logger::getInstance().addSink(&str,DEBUG);
    Logger::getInstance().log(DEBUG) << "message" <<  " plpl" << logend;
    boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] message plpl.{1,2}", boost::regex::perl);
    BOOST_REQUIRE(regex_match(str.str(),regex));
    Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_message2)
{
    std::stringstream str;
    Logger::getInstance().addSink(&str,DEBUG);
    LOG(DEBUG,"message" <<  " plpl" );
    boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] message plpl.{1,2}", boost::regex::perl);
    BOOST_REQUIRE(regex_match(str.str(),regex));
    Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_multipleSink)
{
    std::stringstream str;
    std::stringstream str2;

    Logger::getInstance().addSink(&str,DEBUG);
    Logger::getInstance().addSink(&str2,DEBUG);

    LOG(DEBUG,"message" <<  " plpl" );
    boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] message plpl.{1,2}", boost::regex::perl);
    BOOST_REQUIRE(regex_match(str.str(),regex));
    BOOST_REQUIRE(regex_match(str2.str(),regex));
    Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_moreMessages)
{
    std::stringstream str;
    Logger::getInstance().addSink(&str,DEBUG);
    LOG(DEBUG,"msg1");
    LOG(DEBUG,"msg2");
    boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] msg1.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] msg2.{1,2}", boost::regex::perl);
    BOOST_REQUIRE(regex_match(str.str(),regex));
    Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_forceFlush)
{
    std::stringstream str;
    std::stringstream str2;

    Logger::getInstance().addSink(&str,DEBUG);
    Logger::getInstance().addSink(&str2,DEBUG);

    Logger::setForceFlush(true);

    LOG(DEBUG,"msg1");
    LOG(DEBUG,"msg2");
    boost::regex regex("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] msg1.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] msg2.{1,2}", boost::regex::perl);
    BOOST_REQUIRE(regex_match(str.str(),regex));
    BOOST_REQUIRE(regex_match(str2.str(),regex));
    Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_CASE(log_levels)
{
    std::stringstream str;
    std::stringstream str2;
    std::stringstream str3;

    Logger::getInstance().addSink(&str,DEBUG);
    Logger::getInstance().addSink(&str2,WARN);
    Logger::getInstance().addSink(&str3,ERROR);

    Logger::setLogLevel(DEBUG);

    LOG(DEBUG,"debug");
    LOG(WARN,"warn");
    LOG(ERROR,"error");

    boost::regex regex_3("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] debug.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] warn.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] error.{1,2}", boost::regex::perl);
    boost::regex regex_2("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] warn.{1,2}\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] error.{1,2}", boost::regex::perl);
    boost::regex regex_1("\\[\\d{8}T\\d{6}\\.\\d{6} \\w+\\] error.{1,2}", boost::regex::perl);

    BOOST_CHECK(regex_match(str.str(),regex_3));
    BOOST_CHECK(regex_match(str2.str(),regex_2));
    BOOST_CHECK(regex_match(str3.str(),regex_1));
    Logger::getInstance().destroy();
}

BOOST_AUTO_TEST_SUITE_END();

