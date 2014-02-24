
#include <torrentsync/dht/Address.h>

#include <boost/integer_traits.hpp>

namespace torrentsync
{
namespace dht
{

const time_t Address::good_interval              = 15 * 60;  // 15 minutes
const size_t Address::allowed_unanswered_queries = 10;

Address::Address(const std::string& str)
{
    parse(str);
    setGood();
}

Address::Address( const Address& addr )
{
    *this = addr;
    setGood();
}

}; // dht
}; // torrentsync

