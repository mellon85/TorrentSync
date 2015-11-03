#pragma once

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


/*! Parse a generic message and returns an instance of it.
 * This method must be used to parse messages.
 * @param istream the input stream to read from
 * @return a shared pointer with the message
 * @throw MessageException in case the message was not
 * parsed or missed mantatory parts.
 * @throw MethodUnknownException in case the query received is of an
 * unknown type.
 */
AnyMessage parseMessage(std::istream& istream );
AnyMessage parseMessage(const utils::Buffer& buffer );
AnyMessage parseMessage(const utils::Buffer& buffer, const size_t size);

utils::Buffer getTransactionID(const AnyMessage&);
utils::Buffer getID(const AnyMessage&);
utils::Buffer getType(const AnyMessage&);
std::string getString(const AnyMessage&);

};
};
};
