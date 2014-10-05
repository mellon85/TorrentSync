#pragma once

#include <torrentsync/dht/message/Reply.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/Node.h>
#include <boost/optional.hpp>

#include <vector>

namespace torrentsync
{
namespace dht
{

class NodeData;
class Node;
using namespace torrentsync;

namespace message
{
namespace reply
{

//! Abstract class representing every message
class FindNode : public dht::message::Reply
{
public:
    //! FindNode constructor to initialize the class from a raw data map
    FindNode(const DataMap& dataMap);

    FindNode(FindNode&&) = default;

    FindNode( Message&& );

    FindNode( const Message& );

    //! Destructor
    virtual ~FindNode() = default;

    /** creates a FindNode message reply
     * @param transactionID the ID
     * @param source source address (should be our own address)
     * @param target the target address
     * @param yield a function that returns the closest nodes to send 
     *              until an invalid value is returned
     */
    static const utils::Buffer make( 
        const utils::Buffer& transactionID,
        const dht::NodeData& source,
        const std::function<boost::optional<std::shared_ptr<Node> >()> yield);

    //! returns the parsed nodes
    std::vector<dht::NodeSPtr> getNodes() const;
    
    FindNode& operator=( FindNode&& ) = default;

private:

    void check() const;
};

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */
