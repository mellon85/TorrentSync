
#include <torrentsync/dht/Address.h>

#include <boost/integer_traits.hpp>

#ifndef HAS_Cxx11
#include <boost/typeof/typeof.hpp>
#endif

namespace torrentsync
{
namespace dht
{

const time_t Address::good_interval              = 15 * 60;  // 15 minutes
const time_t Address::allowed_unanswered_queries = 10;

Address::Address(const std::string& str)
{
	parse(str);
    setGood();
}

}; // dht
}; // torrentsync

