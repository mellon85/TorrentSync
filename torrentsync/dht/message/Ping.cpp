#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/AddressData.h>
#include <torrentsync/dht/message/Ping.h>

namespace torrentsync
{
namespace dht
{
namespace message
{

Ping::Ping()
{
}

const std::string&
Ping::getMessage( 
        const std::string& transactionID,
        const AddressData& source,
        std::string& output)
{
    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Arguments);
    enc.startDictionary();
    enc.addDictionaryElement(Field::NodeID,source.string());
    enc.endDictionary();
    enc.addDictionaryElement(Field::QueryType,Messages::Ping); 
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Query); 
    enc.endDictionary();
    output = enc.value();
    return output;
}

} /* message */
} /* dht */
} /* torrentsync */

