#pragma once

#include <algorithm>
#include <list>
#include <string>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <cstdint>
#include <torrentsync/utils/Buffer.h>
#include <stdint.h>

namespace torrentsync {
namespace dht {
namespace message {

using namespace torrentsync;

//! root is always a dictionary
class BEncodeEncoder {
public:
  BEncodeEncoder();

  void addElement(const std::string &v);

  void addDictionaryElement(const std::string &k, const std::string &v);

  void addElement(const utils::Buffer &v);

  void addDictionaryElement(const utils::Buffer &k, const utils::Buffer &v);

  void addDictionaryElement(const std::string &k, const utils::Buffer &v);

  void addDictionaryElement(const utils::Buffer&k, int v);

  void addInteger(uint64_t);

  template <class T> void addList(T&& begin, T&& end);

  void startList();
  void endList();
  void startDictionary();
  void endDictionary();

  utils::Buffer value() const;

private:
  template <class It> void addElement(It begin, const It end);

  template <class It, class It2>
  void addDictionaryElement(It begin1, const It end1, It2 begin2,
                            const It2 end2);

  size_t used_bytes;

  void checkAndExpand(const size_t add);

  mutable utils::Buffer result;

  utils::Buffer lastKey;
};

template <class T> void BEncodeEncoder::addList(T&& begin, T&& end) {
    startList();
    while (begin != end) {
        addElement(*begin);
        ++begin;
    }
    endList();
}

} // torrentsync
} // dht
} // message
