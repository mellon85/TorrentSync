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
using namespace torrentsync::utils;

const Buffer Ping::getQuery( 
    const Buffer& transactionID,
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

const Buffer Ping::getReply( 
    const Buffer& transactionID,
    const torrentsync::dht::NodeData& source)
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
} /* message */
} /* dht */
} /* torrentsync */

