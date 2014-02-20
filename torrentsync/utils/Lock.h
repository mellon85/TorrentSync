#include <boost/thread/shared_mutex.hpp>

typedef boost::shared_mutex Mutex;
typedef boost::unique_lock<Mutex> WriteLock;
typedef boost::shared_lock<Mutex> ReadLock;
typedef boost::upgrade_lock<Mutex> UpgradableLock;
typedef boost::upgrade_to_unique_lock<Mutex> UpgradedWriteLock;

