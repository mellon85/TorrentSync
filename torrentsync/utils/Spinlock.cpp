#include <atomic>
#include <thread>

namespace torrentsync
{
namespace utils
{

const uint32_t concurentThreadsSupported = std::thread::hardware_concurrency();

} /* utils */ 
} /* torrentsync */ 

