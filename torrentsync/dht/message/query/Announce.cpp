#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/query/Announce.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync {
namespace dht {
namespace message {
namespace query {

using torrentsync::utils::DataMap;

static const utils::Buffer PEER_ID =
    Field::Arguments + Field::Separator + Field::PeerID;

using namespace torrentsync;

Announce::Announce(const DataMap &dataMap) : dht::message::Message(dataMap) {
  if (!find(PEER_ID))
    throw MessageException("Missing Peer ID in Announce Reply",
                           ErrorType::protocolError);
}

const utils::Buffer Announce::make(const dht::NodeData &id,
                                  const utils::Buffer &transactionID,
                                  const utils::Buffer &info_hash,
                                  const int port,
                                  const bool implied_port)
{
  BEncodeEncoder enc;
  enc.startDictionary();

  enc.addElement(Field::Arguments);

  enc.startDictionary();
  enc.addDictionaryElement(Field::PeerID, id.write());
  enc.addDictionaryElement(Field::ImpliedPort, static_cast<int>(implied_port));
  enc.addDictionaryElement(Field::InfoHash, info_hash);
  enc.addDictionaryElement(Field::Port, port);
  enc.endDictionary();

  enc.addDictionaryElement(Field::Query, Messages::Announce);
  enc.addDictionaryElement(Field::TransactionID, transactionID);
  enc.addDictionaryElement(Field::Type, Type::Query);

  enc.endDictionary();
  return enc.value();
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */
