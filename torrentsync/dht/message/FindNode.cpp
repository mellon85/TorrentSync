#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/FindNode.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>



namespace torrentsync
{
namespace dht
{
namespace message
{

using namespace torrentsync;

const utils::Buffer
FindNode::getMessage( 
    const utils::Buffer& transactionID,
    const dht::NodeData& source,
    const dht::NodeData& target)
{
    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Arguments);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());
    enc.addDictionaryElement(Field::Target,target.write());
    enc.endDictionary();
    enc.addDictionaryElement(Field::Query,Messages::FindNode); 
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Query); 
    enc.endDictionary();
    return enc.value();
}

const utils::Buffer FindNode::getMessageReply( 
    const utils::Buffer& transactionID,
    const dht::NodeData& source,
    const std::function<boost::optional<dht::NodeSPtr> ()> nodes)
{
    utils::Buffer nodeData(PACKED_NODE_SIZE*DHT_FIND_NODE_COUNT);
    
    size_t bufferIndex = 0;
    auto it = nodes();
    while ( !!it )
    {
        const utils::Buffer data = (*it)->getPackedNode();
        assert(bufferIndex + data.size() <= nodeData.size());
        std::copy(data.cbegin(),data.cend(),nodeData.begin()+bufferIndex);
        bufferIndex += PACKED_NODE_SIZE;
        it = nodes(); // take next
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

utils::Buffer FindNode::getTarget()
{
    boost::optional<utils::Buffer> token;
    token = find( Field::Arguments + "/" + Field::Target );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

utils::Buffer FindNode::getNodes()
{
    boost::optional<utils::Buffer> token;
    token = find( Field::Reply + "/" + Field::Nodes );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

} /* message */
} /* dht */
} /* torrentsync */

