#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/reply/Reply.h>
#include <torrentsync/dht/message/query/Query.h>
#include <boost/variant.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{

typedef boost::variant<
    reply::Reply,
    query::Query
  > AnyMessage;

typedef struct getTransactionID
{

};

};
};
};
