#include <torrentsync/utils/Buffer.h>
#include <boost/asio.hpp>

namespace torrentsync
{
namespace dht
{

namespace TokenManager
{

utils::Buffer getToken( const boost::asio::ip::udp::endpoint& );

bool isTokenValid( const utils::Buffer&, const boost::asio::ip::udp::endpoint& );

} /* TokenManager */ 

} /* dht */ 
} /* torrentsync */
