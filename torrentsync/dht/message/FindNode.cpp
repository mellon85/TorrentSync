#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/FindNode.h>
#include <torrentsync/utils/Buffer.h>

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

const utils::Buffer
FindNode::getMessageReply( 
    const utils::Buffer& transactionID,
    const dht::NodeData& source,
    const dht::NodeData& target,
    const std::function<boost::optional<boost::shared_ptr<NodeData> >()> nodeRetriver)
{
    throw std::runtime_error("Not Implemented Yet");
    BEncodeEncoder enc;
    return enc.value();
}

Buffer FindNode::getTarget()
{
    boost::optional<utils::Buffer> token;
    token = find( Field::Arguments + "/" + Field::Target, data );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

Buffer FindNode::getNodes()
{
    boost::optional<utils::Buffer> token;
    token = find( Field::Reply + "/" + Field::Nodes, data );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

} /* message */
} /* dht */
} /* torrentsync */

