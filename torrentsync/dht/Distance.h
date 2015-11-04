#pragma once

#include <torrentsync/dht/NodeData.h>

namespace torrentsync {
namespace dht {

class Node;

class Distance : public NodeData {
public:
  Distance(){};
  ~Distance(){};

private:
  friend class torrentsync::dht::Node;
};

}; // dht
}; // torrentsync
