#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/utils/Yield.h>

#include <exception> // for not implemented stuff

namespace torrentsync
{
namespace dht
{

using namespace torrentsync;
namespace msg = dht::message;

void RoutingTable::recvMessage(
    const boost::system::error_code& error,
    utils::Buffer buffer,
    std::size_t bytes_transferred,
    const boost::asio::ip::udp::endpoint& sender)
{
    namespace msg = dht::message;
    
    buffer.resize(bytes_transferred);
    LOG(DEBUG,"RoutingTable * from " << sender << " received " <<
        bytes_transferred <<  " " << pretty_print(buffer)
        << " e:" << error.message());

    std::shared_ptr<msg::Message> message;

    // check for errors
    if (error)
    {
        LOG(ERROR, "RoutingTable * recvMessage error: " << error << " " << error.message());
        return;
    }

    // parse the message
    try
    {
        message = msg::Message::parseMessage(buffer,bytes_transferred);
        LOG(DEBUG, "RoutingTable * message parsed: \n" << *message);
    }
    catch ( const msg::MalformedMessageException& e )
    {
        LOG(ERROR, "RoutingTable * message parsing failed: " << pretty_print(buffer) << " e:" << e.what());
        return;
    }

    
    const auto type     = message->getType();

    // fetch the node from the tree table
    boost::optional<NodeSPtr> node = _table.getNode( message->getID() );
    
    if (!!node) // we already know the node
    {
        const auto endpoint = (*node)->getEndpoint();
        // message dropped if the data is still fresh but with a different IP.
        if (!!endpoint && *endpoint != sender)
            return;
    }
    else
    {
        // create new node
        node = boost::optional<NodeSPtr>(NodeSPtr(
            new Node(message->getID(),sender)));
    }

    // if a callback is registered call it instead of the normal flow
    auto callback = getCallback(*message);
    if( !!callback ) 
    {
        callback->call(*message,**node);
    }
    else
    {
        if ( type == msg::Type::Query )
        {
            const auto query = std::dynamic_pointer_cast<msg::Query>(message);
            const auto msg_type = query->getMessageType();
            try
            {
                if ( msg_type == msg::Messages::Ping )
                {
                    handlePingQuery(
                        *std::dynamic_pointer_cast<msg::query::Ping>(message),
                        **node);
                }
                else if ( msg_type == msg::Messages::FindNode )
                {
                    handleFindNodeQuery(
                        *std::dynamic_pointer_cast<msg::query::FindNode>(message),
                        **node);
                } 
                else
                {
                   LOG(ERROR, "RoutingTable * unknown query type: " << pretty_print(buffer) << " - " << message);
                }
            }
            catch ( const std::bad_cast& e ) 
            {
                LOG(ERROR, " RoutingTable * A message was mis-interpreted! " << message << " Report this bug! ");
            }
            catch ( const dht::message::MalformedMessageException& e )
            {
                LOG(ERROR, " RoutingTable * malformed message: " << message);
            }
        }
        else if (type == msg::Type::Reply)
        {
            // Replies should be all managed by a Callback
            // Log the message and drop it, it's an unexpected reply.
            // Maybe it's a reply that came to late or what not.
            LOG(INFO, "RoutingTable * received unexpected reply: \n" << *message << " " << sender);
        }
        else if ( type == msg::Type::Error )
        {
            // @TODO implement error handling
        }
        else 
        {
            LOG(ERROR, "RoutingTable * unknown message type: " << pretty_print(buffer) << " - " << message);
        }
    }
 
    // @TODO post-process
    // - add the node to the known addresses
    // - update node statistics

    // - add the node to the tree in case it's missing, or set it good
    // - update transaction id if it was a query
}

} // dht
} // torrentsync
