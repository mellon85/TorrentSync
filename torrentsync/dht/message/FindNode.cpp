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

using namespace torrentsync::utils;

const torrentsync::utils::Buffer
    FindNode::getMessage( 
        const torrentsync::utils::Buffer& transactionID,
        const torrentsync::dht::NodeData& source,
        const torrentsync::dht::NodeData& target)
{
    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Arguments);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());
    enc.addDictionaryElement(Field::Target,target.write());
    enc.endDictionary();
    enc.addDictionaryElement(Field::QueryType,Messages::FindNode); 
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Query); 
    enc.endDictionary();
    return enc.value();
}

Buffer FindNode::getTarget()
{
    boost::optional<torrentsync::utils::Buffer> token;
    token = find( Field::Arguments + "/" + Field::Target, data );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

Buffer FindNode::getNodes()
{
    boost::optional<torrentsync::utils::Buffer> token;
    token = find( Field::ResponseType + "/" + Field::Nodes, data );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

} /* message */
} /* dht */
} /* torrentsync */

