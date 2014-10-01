#include <torrentsync/utils/Buffer.h>
#include <boost/asio.hpp>

namespace torrentsync
{
namespace dht
{
namespace TokenManager
{

//! @param endpoint the endpoint to generate the token for
//! @return a Token created based on the currently valid seed value
utils::Buffer getToken( const boost::asio::ip::udp::endpoint& endpoint);

//! @param token    Client provided token
//! @param endpoint Client endpoint
//! @return true if the token is valid based on the current and the previous still valid seed values
bool isTokenValid(
    const utils::Buffer& token,
    const boost::asio::ip::udp::endpoint& endpoint);

} /* TokenManager */ 
} /* dht */ 
} /* torrentsync */
