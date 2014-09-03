#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/query/Ping.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{
namespace message
{
namespace query
{

using namespace torrentsync;

Ping::Ping(const DataMap& dataMap) : dht::message::Query(dataMap)
{
    if (!find(Field::Arguments + "/" + Field::PeerID))
        throw MalformedMessageException("Missing Peer ID in Ping Reply",
                ErrorType::protocolError);
}

const utils::Buffer Ping::make( 
    const utils::Buffer& transactionID,
    const torrentsync::dht::NodeData& source)
{
    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Arguments);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());
    enc.endDictionary();
    enc.addDictionaryElement(Field::Query,Messages::Ping); 
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Query);
    enc.endDictionary();
    return enc.value();
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */

