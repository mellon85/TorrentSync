#include <torrentsync/utils/log/Logger.h>

using namespace torrentsync::utils::log;

int main()
{
    Logger::getInstance().addSink(&std::cerr,DEBUG);
    Logger::getInstance().setLogLevel(DEBUG);

    return 0;
}
