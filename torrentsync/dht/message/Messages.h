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


};
};
};
