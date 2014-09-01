#include <torrentsync/dht/message/Reply.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

Reply::~Reply() {}

Reply::Reply( const DataMap& data ) : Message(data) {}

} /* message */
} /* dht */
} /* torrentsync */

