#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync {
namespace dht {

class NodeData;

namespace message {
namespace reply {

//! Abstract class representing every message
class Error : public dht::message::Message {
public:
  //! Ping constructor to initialize the class from a raw data map
  Error(const torrentsync::utils::DataMap &dataMap);

  Error(Error &) = default;
  Error(Error &&) = default;

  Error &operator=(Error &&) = default;
  Error &operator=(const Error &) = default;

  //! Destructor
  ~Error() = default;

  //! creates a Error message
  //! @param transactionID the ID
  //! @param source source address (should be our own address)
  static const utils::Buffer make(const utils::Buffer &transactionID,
                                  const ErrorType::error_type error);

private:
  void check() const;
};

} // query
} // message
} // dht
} // torrentsync
