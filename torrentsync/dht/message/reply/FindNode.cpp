#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/reply/FindNode.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>

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
    if (!find(Field::Reply + "/" + Field::PeerID))
        throw MalformedMessageException("Missing nodes in find_node reply");
    if (!find(Field::Reply + "/" + Field::Nodes))
        throw MalformedMessageException("Missing nodes in find_node reply");
}

const utils::Buffer FindNode::make( 
    const utils::Buffer& transactionID,
    const dht::NodeData& source,
    const std::function<boost::optional<dht::NodeSPtr> ()> nodes)
{
    utils::Buffer nodeData(PACKED_NODE_SIZE*DHT_FIND_NODE_COUNT);
    
    size_t bufferIndex = 0;
    for ( auto it = nodes(); !!it; it = nodes() )
    {
        const utils::Buffer data = (*it)->getPackedNode();
        assert(bufferIndex + data.size() <= nodeData.size());
        std::copy(data.cbegin(),data.cend(),nodeData.begin()+bufferIndex);
        bufferIndex += PACKED_NODE_SIZE;
    }
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

utils::Buffer FindNode::getNodes()
{
    auto token = find( Field::Reply + "/" + Field::Nodes );
    assert(!!token);
    return *token;
}

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */

