#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/message/reply/Error.h>
#include <torrentsync/utils/Buffer.h>

#include <boost/lexical_cast.hpp>

#include <map>
#include <utility>

namespace torrentsync {
namespace dht {
namespace message {
namespace reply {

using torrentsync::utils::DataMap;

using namespace torrentsync;

static const std::map<ErrorType::error_type, std::string> stringMessage = {
    {ErrorType::genericError, "Generic Error"},
    {ErrorType::serverError, "Server Error"},
    {ErrorType::protocolError, "Protocol Error"},
    {ErrorType::methodUnknownError, "Method Unknown"}};

Error::Error(const DataMap &dataMap) : dht::message::Message(dataMap) {
  check();
}

const utils::Buffer
Error::make(const utils::Buffer &transactionID,
            const dht::message::ErrorType::error_type error) {
  BEncodeEncoder enc;
  enc.startDictionary();
  enc.addElement(Field::Error);
  enc.startList();
  enc.addInteger(static_cast<int64_t>(error));
  enc.addElement(utils::makeBuffer(stringMessage.at(error)));
  enc.endList();
  enc.addDictionaryElement(Field::TransactionID, transactionID);
  enc.addDictionaryElement(Field::Type, Type::Error);
  enc.endDictionary();
  return enc.value();
}

void Error::check() const {
  if (!find(Field::Error + "/0"))
    throw MessageException("Missing error code Error Reply",
                           ErrorType::protocolError);
  if (!find(Field::Error + "/1"))
    throw MessageException("Missing error message in Error Reply",
                           ErrorType::protocolError);
}

} /* query */
} /* message */
} /* dht */
} /* torrentsync */
