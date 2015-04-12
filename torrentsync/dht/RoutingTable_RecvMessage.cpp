#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/utils/log/Logger.h>

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
    LOG(DEBUG,"Routingtable * received a message from " << sender << " e:"
            << error.message());
    LOG(DATA,"RoutingTable * " << sender << " received " <<
        bytes_transferred <<  " " << pretty_print(buffer));

    std::unique_ptr<msg::Message> message;

    // check for errors
    if (error)
    {
        LOG(ERROR, "RoutingTable * recvMessage error: " << error << " " << error.message());
        return;
    }

    // parse the message
    try
    {
        message = msg::Message::parseMessage(buffer);
        LOG(DATA, "RoutingTable * message parsed: \n" << *message);
    }
    catch ( const msg::MessageException& e )
    {
        LOG(ERROR, "RoutingTable * message parsing failed" << e.what());
        LOG(DATA, "RoutingTable * message parsing failed: " << pretty_print(buffer));
        // @TODO send malformed message error
        return;
    }

    const auto type = message->getType();

    // fetch the node from the tree table
    boost::optional<NodeSPtr> node = _table.getNode( NodeData(message->getID()) );

    if (!!node) // we already know the node
    {
        const auto endpoint = (*node)->getEndpoint();
        // message dropped if t he data is still fresh but with a different IP.
        if (!!endpoint && *endpoint != sender)
            return;
    }
    else
    {
        // create new node
        LOG(DEBUG, "New node: " << pretty_print(message->getID()) << " addr: " << sender);
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
        LOG(DEBUG,"Callback not found");
        if ( type == msg::Type::Query )
        {
            const auto query = dynamic_cast<msg::Query*>(message.get());
            assert(query != nullptr);
            const auto msg_type = query->getMessageType();
            try
            {
                if ( msg_type == msg::Messages::Ping )
                {
                    assert(dynamic_cast<msg::query::Ping*>(query) != nullptr);
                    handlePingQuery(
                        dynamic_cast<msg::query::Ping&>(*query),
                        **node);
                }
                else if ( msg_type == msg::Messages::FindNode )
                {
                    assert(dynamic_cast<msg::query::FindNode*>(query) != nullptr);
                    handleFindNodeQuery(
                        dynamic_cast<msg::query::FindNode&>(*query),
                        **node);
                } 
                else
                {
                   LOG(ERROR, "RoutingTable * unknown query type: " << pretty_print(buffer) << " - " << *message);
                }
            }
            catch ( const std::bad_cast& e )
            {
                LOG(ERROR, " RoutingTable * A message was mis-interpreted! " << *message << " Report this bug! ");
            }
            catch ( const dht::message::MessageException& e )
            {
                LOG(ERROR, " RoutingTable * malformed message: " << *message);
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
            // same be behaviour as a Reply without a callback
            LOG(WARN, "RoutingTable * received unexpected error: \n" << *message << " " << sender);
        }
        else 
        {
            LOG(ERROR, "RoutingTable * unknown message type: " << pretty_print(buffer) << " - " << *message);
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
