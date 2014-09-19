#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/query/FindNode.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>

namespace torrentsync
{
namespace dht
{
namespace message
{
namespace query
{

using namespace torrentsync;

FindNode::FindNode(const DataMap& dataMap) : Query(dataMap)
{
    if (!find( Field::Arguments + "/" + Field::Target ))
        throw MessageException("Couldn't find Target",
                ErrorType::protocolError);
}

const utils::Buffer FindNode::make( 
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

utils::Buffer FindNode::getTarget()
{
    auto token = find( Field::Arguments + "/" + Field::Target );
    assert(!!token);
    return *token;
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */

