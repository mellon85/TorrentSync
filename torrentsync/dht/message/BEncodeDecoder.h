#pragma once

#include <list>
#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <boost/optional.hpp>

#include <torrentsync/utils/Buffer.h>

namespace torrentsync {
namespace dht {
namespace message {

using namespace torrentsync;

typedef std::map<utils::Buffer, utils::Buffer> DataMap;

typedef std::runtime_error BEncodeException;

//! Decoder for a bencoded message
class BEncodeDecoder {
public:
  //! Constructor
  BEncodeDecoder() {}

  //! Destructor
  ~BEncodeDecoder() {}

  /*! Parse the message from a istream
   *  @param stream stream parser
   */
  void parseMessage(std::istream &stream);

  const DataMap &getData() const noexcept { return data; }

  boost::optional<utils::Buffer> find(const utils::Buffer &key) const;

private:
  //! parse a single element from the stream
  utils::Buffer readElement(std::istream &stream);

  //! parse a single value from the stream
  utils::Buffer readValue(std::istream &stream);

  typedef enum { DICTIONARY = 0, LIST } structure_t;

  //! Stack entry
  typedef std::pair<utils::Buffer, structure_t> structureStackE;

  //! Stack to use while parsing
  std::vector<structureStackE> structureStack;

  //! Container of all the parsed data
  DataMap data;

  utils::Buffer appendPath();
};

} // torrentsync
} // dht
} // message
