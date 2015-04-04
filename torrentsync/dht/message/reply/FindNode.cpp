#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>
#include <torrentsync/utils/log/Logger.h>

#include <torrentsync/utils/Yield.h>

namespace torrentsync
{
namespace dht
{
namespace message
{
namespace reply
{

using namespace torrentsync;

static const utils::Buffer NODES = Field::Reply + Field::Separator + Field::Nodes;
static const utils::Buffer PEER_ID = Field::Reply + Field::Separator + Field::PeerID;

FindNode::FindNode(const DataMap& dataMap) : dht::message::Reply(dataMap)
{
    check();
}

const utils::Buffer FindNode::make(
    const utils::Buffer& transactionID,
    const dht::NodeData& source,
    const std::function<boost::optional<dht::NodeSPtr> ()> nodes)
{
    utils::Buffer nodeData;
    nodeData.reserve(PACKED_NODE_SIZE*DHT_FIND_NODE_COUNT);

    utils::for_each(nodes, [&](const dht::NodeSPtr& it)
    {
        nodeData += it->getPackedNode();
    });

    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Reply);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());
    enc.addDictionaryElement(Field::Nodes,nodeData);
    enc.endDictionary();
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Reply);
    enc.endDictionary();
    return enc.value();
}

std::vector<dht::NodeSPtr> FindNode::getNodes() const
{
    auto token = find(NODES);
    assert(!!token);

    const utils::Buffer& buff = *token;

    std::vector<dht::NodeSPtr> nodes;

    try
    {
        NodeSPtr last_node;
        for( auto it = buff.begin();
                it < buff.end(); // enough data for a whole node
                it += PACKED_NODE_SIZE )
        {
            NodeSPtr node(new Node(it, it+PACKED_NODE_SIZE));
            if (last_node && *last_node == *node)
            {
                LOG(DEBUG, "Duplicated node: " << *node);
                continue;
            }
            LOG(DEBUG, "Added node: " << *node);
            nodes.push_back(node);
            last_node = node;
        }
    }
    catch (std::invalid_argument& e)
    {
        LOG(ERROR, "Node parsing failed. " << e.what());
    }

    return nodes;
}

void FindNode::check() const
{
    if (!find(PEER_ID))
        throw MessageException("Missing nodes in find_node reply",
                ErrorType::protocolError);
    if (!find(NODES))
        throw MessageException("Missing nodes in find_node reply",
                ErrorType::protocolError);
}

FindNode::FindNode( Message&& m ) : Reply(std::move(m))
{
    check();
}

FindNode::FindNode( const Message& m ) : Reply(m)
{
    check();
}

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */
