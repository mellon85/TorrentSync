#include <torrentsync/dht/Callback.h>
#include <torrentsync/dht/message/Message.h>

#include <ctime>

namespace torrentsync
{
namespace dht
{

const size_t Callback::TIME_LIMIT = 3*60;

Callback::Callback(
    const callback& function,
    const std::string& type,
    const std::string& messageType,
    const torrentsync::dht::NodeData& source,
    const filterTransactionID& transactionID ) :
        _func(function), _type(type), _messageType(messageType),
        _source(source), _transactionID(transactionID),
        _creation_time(time(NULL))
{
}

bool Callback::isOld() const
{
   return difftime(time(NULL),_creation_time) > TIME_LIMIT;
}

bool Callback::verifyConstraints( const torrentsync::dht::message::Message& message ) const
{
    if ((message.getType() != _type) || (message.getMessageType() != _messageType))
        return false;

    if ( _source != message.getID())
        return false;
    
    if ( !!_transactionID && !( *_transactionID == message.getTransactionID()))
        return false;

    return true;
}

void Callback::call( const torrentsync::dht::message::Message& m ) const
{
    _func(m);
}

} // dht
} // torrentsync
