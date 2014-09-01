#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/reply/Ping.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{
namespace message
{
namespace reply
{

using namespace torrentsync;

Ping::Ping(const DataMap& dataMap) : dht::message::Reply(dataMap)
{
    if (!find(Field::Reply + "/" + Field::PeerID))
        throw MalformedMessageException("Missing Peer ID in Ping Reply");
}

const utils::Buffer Ping::make( 
    const utils::Buffer& transactionID,
    const dht::NodeData& source)
{
    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Reply);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());
    enc.endDictionary();
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Reply);
    enc.endDictionary();
    return enc.value();
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */

