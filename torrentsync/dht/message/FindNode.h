#pragma once

#include <torrentsync/dht/message/Message.h>
#include <torrentsync/utils/Buffer.h>
#include <boost/optional.hpp>

namespace torrentsync
{
namespace dht
{

class NodeData;
class Node;
using namespace torrentsync::utils;

namespace message
{

//! Abstract class representing every message
class FindNode : public Message
{
public:
    //! FindNode constructor to initialize the class from a raw data map
    FindNode(const DataMap& dataMap) : Message(dataMap) {}

    //! Destructor
    virtual ~FindNode() {}

    /** creates a FindNode message
     * @param transactionID the ID
     * @param source source address (should be our own address)
     * @param target the target address
     */
    static const torrentsync::utils::Buffer getMessage( 
        const torrentsync::utils::Buffer& transactionID,
        const torrentsync::dht::NodeData& source,
        const torrentsync::dht::NodeData& target);

    /** creates a FindNode message reply
     * @param transactionID the ID
     * @param source source address (should be our own address)
     * @param target the target address
     * @param yield a function that returns the closest nodes to send 
     *              until an invalid value is returned
     */
    static const torrentsync::utils::Buffer getMessageReply( 
        const torrentsync::utils::Buffer& transactionID,
        const torrentsync::dht::NodeData& source,
        const torrentsync::dht::NodeData& target,
        const std::function<boost::optional<boost::shared_ptr<NodeData> >()> yield);

    Buffer getTarget();

    Buffer getNodes();
};

} /* message */
} /* dht */
} /* torrentsync */
