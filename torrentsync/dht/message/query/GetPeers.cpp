#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/query/GetPeers.h>
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

GetPeers::GetPeers(const DataMap& dataMap) : dht::message::Query(dataMap)
{
    if (!find(Field::Arguments + "/" + Field::PeerID))
        throw MessageException("Missing Routing Table ID in GetPeers Query",
                ErrorType::protocolError);

    if (!find(Field::Arguments + "/" + Field::InfoHash))
        throw MessageException("Missing Info Hash ID in GetPeers Query",
                ErrorType::protocolError);
}

const utils::Buffer GetPeers::make(
    const utils::Buffer& transactionID,
    const utils::Buffer& infoHash,
    const torrentsync::dht::NodeData& source)
{
    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Arguments);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());
    enc.addDictionaryElement(Field::InfoHash,infoHash);
    enc.endDictionary();
    enc.addDictionaryElement(Field::Query,Messages::GetPeers); 
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Query);
    enc.endDictionary();
    return enc.value();
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */

