#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/App.h>
#include <iostream>

using namespace torrentsync::utils::log;

int main() {
  Logger::getInstance().addSink(
      std::unique_ptr<std::ostream>(new std::ostream(std::cerr.rdbuf())),
      DEBUG);

  // @TODO parsing the command line arguments and environment variable
  // should let the main function initialize app with the parameters
  // necessary to load the configuration

  torrentsync::App app;
  app.runloop();

  return 0;
}
