#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/Node.h>
#include <boost/optional.hpp>


namespace torrentsync
{
namespace dht
{

class NodeData;
class Node;
using namespace torrentsync;

namespace message
{
namespace query
{

//! Abstract class representing every message
class FindNode : public dht::message::Message
{
public:
    //! FindNode constructor to initialize the class from a raw data map
    FindNode(const DataMap& dataMap);

    FindNode(FindNode&&) = default;
    FindNode(const FindNode&) = default;

    //! Destructor
    ~FindNode() = default;

    /** creates a FindNode message
     * @param transactionID the ID
     * @param source source address (should be our own address)
     * @param target the target address
     */
    static const utils::Buffer make(
        const utils::Buffer& transactionID,
        const dht::NodeData& source,
        const dht::NodeData& target);

    //! returns the target node
    utils::Buffer getTarget();

    FindNode& operator=( FindNode&& ) = default;
    FindNode& operator=( const FindNode& ) = default;
};

} /* query */
} /* message */
} /* dht */
} /* torrentsync */
