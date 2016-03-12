#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/utils/Buffer.h>

namespace torrentsync {
namespace dht {

class NodeData;

namespace message {
namespace query {

//! Abstract class representing every message
class Announce : public dht::message::Message {
public:
  //! Announce constructor to initialize the class from a raw data map
  Announce(const torrentsync::utils::DataMap &dataMap);

  Announce(Announce &&) = default;
  Announce(const Announce &) = default;

  //! Destructor
  ~Announce() = default;

  //! creates a Announce message
  //! @param id The current node ID
  //! @param transactionID the ID
  //! @param info_hash Torrent info hash
  //! @param port the port to be contacted on
  //! @param implid_port use the source UDP port instead of port
  static const utils::Buffer make(const dht::NodeData &id,
                                  const utils::Buffer &transactionID,
                                  const utils::Buffer &info_hash,
                                  const int port,
                                  const bool implied_port = false);

  Announce &operator=(Announce &&) = default;
  Announce &operator=(const Announce &) = default;
};

} /* query */
} /* message */
} /* dht */
} /* torrentsync */
