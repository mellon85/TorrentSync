#include <torrentsync/utils/log/Logger.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/RoutingTable.h>
#include <torrentsync/dht/message/Ping.h>
#include <torrentsync/dht/message/FindNode.h>
#include <torrentsync/dht/Callback.h>
#include <torrentsync/utils/Yield.h>

#include <boost/cast.hpp>

#include <exception> // for not implemented stuff

namespace torrentsync
{
namespace dht
{

using namespace torrentsync;
namespace msg = dht::message;

void RoutingTable::handlePingQuery(
    const dht::message::Ping& ping,
    const dht::Node&          node)
{
    assert(!!(node.getEndpoint()));
    
    // send ping reply
    sendMessage( msg::Ping::getReply(
                    ping.getTransactionID(), _table.getTableNode()),
                 *(node.getEndpoint()) );
}

//! Handle ping reply.
void RoutingTable::handlePingReply(
    const dht::message::Ping& message,
    const dht::Node& node)
{
    LOG(DEBUG,"Ping Reply received " << message.getID() << " " << node);
}

void RoutingTable::handleFindNodeQuery(
    const dht::message::FindNode& message,
    const dht::Node& node)
{
    LOG(DEBUG,"Find Query received " << message.getID() << " " << node);
    assert(!!(node.getEndpoint()));

    auto nodes = _table.getClosestNodes(node);
    sendMessage(
        msg::FindNode::getMessageReply(
            message.getTransactionID(),
            _table.getTableNode(),
            utils::makeYield<dht::NodeSPtr>(nodes.cbegin(),nodes.cend()).function()),
        *(node.getEndpoint()));
}

void RoutingTable::handleFindNodeReply(
    const dht::message::FindNode& message,
    const dht::Node& node)
{
    LOG(WARN,"find_node reply without a callback. " << message.getID() << "-" << node );
}


void RoutingTable::doPing(
    dht::Node& destination )
{
    assert(!!(destination.getEndpoint()));

    torrentsync::utils::Buffer transaction = newTransaction();

    torrentsync::utils::Buffer ping = msg::Ping::getQuery(
        transaction,
        _table.getTableNode());

    registerCallback([&](
            boost::optional<Callback::callback_payload_t> data,
            const torrentsync::dht::Callback&             trigger) -> void {

            if (!!data)
            {
                std::get<1>(*data).setGood(); // mark the node as good
                LOG(DEBUG,"Ping handled: " << std::get<1>(*data) );
            }
        }, destination, transaction);

    // send ping reply
    sendMessage( ping, *(destination.getEndpoint()) );
}

} // dht
} // torrentsync