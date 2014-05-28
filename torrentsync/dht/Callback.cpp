#include <torrentsync/dht/Callback.h>

#include <ctime>


namespace torrentsync
{
namespace dht
{

const size_t Callback::TIME_LIMIT = 3*60;

Callback::Callback(
    const callback& function,
    const std::string& type,
    const filterPeer& source,
    const filterTransactionID& transactionID ) :
        _func(function), _type(type), _source(source), _transactionID(transactionID),
        _creation_time(time(NULL))
{
}

bool Callback::isOld() const
{
   return difftime(time(NULL),_creation_time) > TIME_LIMIT;
}

bool Callback::verifyConstraints( const torrentsync::dht::message::Message& message ) const
{
    return false;
}

} // dht
} // torrentsync
