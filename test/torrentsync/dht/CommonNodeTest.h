#pragma once

#include <string>

#define TEST_LOOP_COUNT 100

static inline std::string
generateRandomNode(const std::string &prefix = std::string()) {
  std::string data;
  data.reserve(40);
  data.append(prefix);
  assert(prefix.length() < 40);
  while (data.length() < 40) {
    int v = rand() % 16;
    data +=
        static_cast<char>(v + (v < 10 ? '0' : ('a' - 10))); // either 0-9 or a-f
  }
  return data;
}