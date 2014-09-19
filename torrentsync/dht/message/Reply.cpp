#include <torrentsync/dht/message/Reply.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

Reply::~Reply()
{
}

Reply::Reply( const DataMap& data ) : Message(data)
{
}

Reply::Reply( Message&& m ) : Message(std::move(m))
{
}

Reply::Reply( const Message& m ) : Message(m)
{
}

} /* message */
} /* dht */
} /* torrentsync */

