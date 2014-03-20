#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/Ping.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

const torrentsync::utils::Buffer
Ping::getMessage( 
    const torrentsync::utils::Buffer& transactionID,
    const torrentsync::dht::NodeData& source)
{
    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Arguments);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());
    enc.endDictionary();
    enc.addDictionaryElement(Field::QueryType,Messages::Ping); 
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Query); 
    enc.endDictionary();
    return enc.value();
}

torrentsync::utils::Buffer Ping::getID()
{
    static const std::string path = Field::Arguments + "/" + Field::PeerID;
    boost::optional<torrentsync::utils::Buffer> id;
    id = find(path);
    if (!id)
        throw MalformedMessageException("Couldn't find node id");
    return *id;
}

torrentsync::utils::Buffer Ping::getToken()
{
    boost::optional<torrentsync::utils::Buffer> token;
    token = find( Field::TransactionID );
    if (!token)
        throw MalformedMessageException("Couldn't find token");
    return *token;
}

} /* message */
} /* dht */
} /* torrentsync */

