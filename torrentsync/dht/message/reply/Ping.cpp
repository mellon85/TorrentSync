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

static const utils::Buffer PEER_ID = Field::Reply + Field::Separator + Field::PeerID;

using namespace torrentsync;

Ping::Ping(const DataMap& dataMap) : dht::message::Reply(dataMap)
{
    check();
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

Ping::Ping( Message&& m ) : Reply(std::move(m))
{
    check();
}

Ping::Ping( const Message& m ) : Reply(m)
{
    check();
}

void Ping::check() const
{
    if (!find(PEER_ID))
        throw MessageException("Missing Peer ID in Ping Reply",
                ErrorType::protocolError);
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */
