#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/message/Message.h>

#include <ctime>

namespace torrentsync
{
namespace dht
{

const size_t Callback::TIME_LIMIT = 3*60;

Callback::Callback(
    const callback_t& callback,
    const boost::optional<dht::NodeData>& source,
    const utils::Buffer& transactionID ) :
        _callback(callback),
        _source(source),
        _transactionID(transactionID),
        _creation_time(time(NULL))
{
}

bool Callback::isOld() const
{
   return difftime(time(NULL),_creation_time) > TIME_LIMIT;
}

bool Callback::verifyConstraints( const dht::message::Message& message ) const
{
    if ( !!_source && *_source != NodeData(message.getID()) )
        return false;

    if ( !(_transactionID == message.getTransactionID()) )
        return false;

    return true;
}

void Callback::call(
    const dht::message::Message& m,
    dht::Node& node) const
{
    _callback(
        payload_type(m,node),*this);
}

void Callback::timeout() const
{
    _callback(
        boost::optional<payload_type>(), *this);
}

bool Callback::operator==( const Callback& c ) const
{
    const bool equality = _transactionID == c._transactionID;
    if (equality)
    {
        assert( _source == c._source);
        assert( _creation_time == c._creation_time);
    }
    return equality;
}

bool Callback::operator<=( const Callback& c ) const
{
    return _transactionID <= c._transactionID;
}

} // dht
} // torrentsync
