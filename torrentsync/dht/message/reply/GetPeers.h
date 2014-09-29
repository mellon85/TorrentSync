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
class GetPeers : public dht::message::Reply
{
public:
    typedef enum {
        Nodes,
        Peers
    } ParameterType;

    //! GetPeers constructor to initialize the class from a raw data map
    GetPeers(const DataMap& dataMap);

    GetPeers(GetPeers&&) = default;

    GetPeers( Message&& );

    GetPeers( const Message& );
    
    //! Destructor
    virtual ~GetPeers() = default;

    /** creates a GetPeers message reply
     * @param transactionID the ID
     * @param source source address (should be our own address)
     * @param target the target address
     * @param yield a function that returns the closest nodes to send 
     *              until an invalid value is returned
     * @param nodesOrPeers if true sends the nodes informations, otherwise peers
     */
    static const utils::Buffer make( 
        const utils::Buffer& transactionID,
        const dht::NodeData& source,
        const std::function<boost::optional<std::shared_ptr<Node> >()> yield,
        const ParameterType nodesOrPeers); // for the K closest
        
    //! returns the parsed nodes
    boost::optional<std::vector<dht::NodeSPtr> > getNodes() const;
    
    //! TODO return endpoints
    boost::optional<std::vector< udp::endpoint > > getPeers() const;
    
    GetPeers& operator=( GetPeers&& ) = default;

private:

    void check() const;
};

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */
