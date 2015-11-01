#pragma once

#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/Messages.h>

#include <functional>

namespace torrentsync
{
namespace dht
{
class Node;

namespace message
{
class Message;
}

/*  Callback constains a function to call when a message has arrived matching
 *  specific constraints.  In this way we can easily write asynchronous code on
 *  the server side to process either single query-reply DHT messages, and also
 *  more complex sessions.
 *
 *  This class is kind of generic, in the future it may be modified and moved
 *  to a more generic place in case it's needed by other parts of the
 *  applications.
 *
 *  Callback object default order is based on the transaction ID
 **/

using namespace torrentsync;

class Callback
{
public:
    //! after 3 minutes a callback is condisered old
    static const size_t TIME_LIMIT;

    typedef struct T {
        template <class M, class N>
        T(M&& m, N&& n) : message(std::forward<M>(m)),
                          node(std::forward<N>(n)) {}

        const dht::message::AnyMessage& message;
        dht::Node& node; } payload_type;

    //! type of the callback
    typedef std::function<void (
            boost::optional<payload_type>,
            const dht::Callback&)> callback_t;

    //! Constructor
    //! @param callback         Callback to call
    //! @param source           Filter by the source address
    //! @param transactionID    Filter by transaction ID
    Callback(
        const callback_t& callback,
        const boost::optional<dht::NodeData>& source,
        const utils::Buffer& transactionID);

    Callback( const Callback& ) = default;
    Callback( Callback&& ) = default;

    bool operator==( const Callback& ) const;

    bool operator<=( const Callback& ) const;

    Callback& operator=( Callback&& ) = default;

    Callback& operator=( const Callback& ) = default;

    //! Calls the callback function
    void call(
        const dht::message::Message&,
        dht::Node& ) const;

    void timeout() const;

    //! verifies if the callback is too old.
    //! @returns true if the callback is more than TIME_LIMIT seconds old.
    bool isOld() const noexcept;

    //! verifies if the callback respects the filters set
    bool verifyConstraints(
        const dht::message::Message& message ) const noexcept;

private:

    //! callback function to call
    callback_t _callback;

    //! filter condition for source address
    boost::optional<dht::NodeData> _source;

    //! filter condition for transaction it
    utils::Buffer _transactionID;

    //! creation time, to filter out old callbacks
    time_t _creation_time;
};

}; // dht
}; // torrentsync
