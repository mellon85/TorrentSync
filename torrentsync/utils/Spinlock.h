#include <atomic>
#include <thread>

namespace torrentsync
{
namespace utils
{

extern const uint32_t concurentThreadsSupported;

class Spinlock
{
public:

    Spinlock()                             = default;
    Spinlock( Spinlock&& )                 = default;
    Spinlock& operator=( Spinlock&& )      = default;
    Spinlock( const Spinlock& )            = delete;
    Spinlock& operator=( const Spinlock& ) = delete;

    void lock() noexcept {
        while( !flag.test_and_set(std::memory_order_acquire) )
            if (concurentThreadsSupported == 1) // follow the hint
                std::this_thread::yield();
    }

    void unlock() noexcept { 
        flag.clear(std::memory_order_release);
    }

    bool try_lock() noexcept {
        return flag.test_and_set(std::memory_order_acquire);
    }

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

};

} /* utils */ 
} /* torrentsync */ 
