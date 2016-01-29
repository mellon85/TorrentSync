#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/query/GetPeers.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync {
namespace dht {
namespace message {
namespace query {

static const utils::Buffer INFO_HASH =
    Field::Arguments + Field::Separator + Field::InfoHash;
static const utils::Buffer PEER_ID =
    Field::Arguments + Field::Separator + Field::PeerID;

using torrentsync::utils::DataMap;

using namespace torrentsync;

GetPeers::GetPeers(const DataMap &dataMap) : dht::message::Message(dataMap) {
  if (!find(PEER_ID))
    throw MessageException("Missing Routing Table ID in GetPeers Query",
                           ErrorType::protocolError);

  if (!find(INFO_HASH))
    throw MessageException("Missing Info Hash ID in GetPeers Query",
                           ErrorType::protocolError);
}

const utils::Buffer GetPeers::make(const utils::Buffer &transactionID,
                                   const utils::Buffer &infoHash,
                                   const torrentsync::dht::NodeData &source) {
  BEncodeEncoder enc;
  enc.startDictionary();
  enc.addElement(Field::Arguments);
  enc.startDictionary();
  enc.addDictionaryElement(Field::PeerID, source.write());
  enc.addDictionaryElement(Field::InfoHash, infoHash);
  enc.endDictionary();
  enc.addDictionaryElement(Field::Query, Messages::GetPeers);
  enc.addDictionaryElement(Field::TransactionID, transactionID);
  enc.addDictionaryElement(Field::Type, Type::Query);
  enc.endDictionary();
  return enc.value();
}

utils::Buffer GetPeers::getInfoHash() const {
  auto result = find(INFO_HASH);
  assert(!!result);
  return *result;
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */
