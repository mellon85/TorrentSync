#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/dht/message/reply/Reply.h>
#include <torrentsync/dht/message/query/Query.h>

#include <boost/variant.hpp>
#include <boost/variant/apply_visitor.hpp>

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

//! generic accessor to obtain the transaction id of the message
typedef struct __getTransactionID__T : public boost::static_visitor<utils::Buffer>
{

utils::Buffer operator()(const reply::Reply& r) const
{
    return boost::apply_visitor(*this, r);
}

utils::Buffer operator()(const query::Query& q) const
{
    return boost::apply_visitor(*this, q);
}

template <class T>
utils::Buffer operator()(T&& m) const
{
    return m.getTransactionID();
}

} getTransactionID;

//! generic accessor to obtain the id of the message
typedef struct __getID__T : public boost::static_visitor<utils::Buffer>
{

utils::Buffer operator()(const reply::Reply& r) const
{
    return boost::apply_visitor(*this, r);
}

utils::Buffer operator()(const query::Query& q) const
{
    return boost::apply_visitor(*this, q);
}

template <class T>
utils::Buffer operator()(T&& m) const
{
    return m.getID();
}

} getID;

//! generic accessor to obtain the id of the message
typedef struct __string__T : public boost::static_visitor<std::string>
{

std::string operator()(const reply::Reply& r) const
{
    return boost::apply_visitor(*this, r);
}

std::string operator()(const query::Query& q) const
{
    return boost::apply_visitor(*this, q);
}

template <class T>
std::string operator()(T&& m) const
{
    return m.string();
}

} getString;

//! generic accessor to obtain the transaction id of the message
typedef struct __getType__T : public boost::static_visitor<utils::Buffer>
{

utils::Buffer operator()(const reply::Reply& r) const
{
    return boost::apply_visitor(*this, r);
}

utils::Buffer operator()(const query::Query& q) const
{
    return boost::apply_visitor(*this, q);
}

template <class T>
utils::Buffer operator()(T&& m) const
{
    return m.getType();
}

} getType;


};
};
};
