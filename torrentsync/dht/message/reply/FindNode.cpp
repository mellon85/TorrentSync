#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>

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

FindNode::FindNode(const DataMap& dataMap) : dht::message::Reply(dataMap)
{
    check();
}

const utils::Buffer FindNode::make( 
    const utils::Buffer& transactionID,
    const dht::NodeData& source,
    const std::function<boost::optional<dht::NodeSPtr> ()> nodes)
{
    utils::Buffer nodeData(PACKED_NODE_SIZE*DHT_FIND_NODE_COUNT);
    
    size_t bufferIndex = 0;
    utils::for_each(nodes, [&](const dht::NodeSPtr& it)
    {
        const utils::Buffer data = it->getPackedNode();
        assert(bufferIndex + data.size() <= nodeData.size());
        std::copy(data.cbegin(),data.cend(),nodeData.begin()+bufferIndex);
        bufferIndex += PACKED_NODE_SIZE;
    });
    nodeData.resize(bufferIndex);
    
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
    auto token = find( Field::Reply + "/" + Field::Nodes );
    assert(!!token);

    const utils::Buffer& buff = *token;

    std::vector<dht::NodeSPtr> nodes;
    
    for( auto it = buff.begin(); it+PACKED_NODE_SIZE <= buff.end(); it += PACKED_NODE_SIZE )
    {
        nodes.push_back(NodeSPtr(new Node(it,it+PACKED_NODE_SIZE)));
    }
   
    return nodes;
}

void FindNode::check() const
{
    if (!find(Field::Reply + "/" + Field::PeerID))
        throw MalformedMessageException("Missing nodes in find_node reply",
                ErrorType::protocolError);
    if (!find(Field::Reply + "/" + Field::Nodes))
        throw MalformedMessageException("Missing nodes in find_node reply",
                ErrorType::protocolError);
}

FindNode::FindNode( Message&& m ) : Reply(m)
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

