#pragma once

#include <torrentsync/dht/message/Message.h>
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
class GetPeers : public dht::message::Message
{
public:

    //! GetPeers constructor to initialize the class from a raw data map
    GetPeers(const DataMap& dataMap);

    GetPeers(GetPeers&&) = default;
    GetPeers(const GetPeers&) = default;

    //! Destructor
    ~GetPeers() = default;

    /** creates a GetPeers message reply
     * @param transactionID the ID
     * @param token the authentication token from previous messages
     * @param source source address (should be our own address)
     * @param target the target address
     * @param yield a function that returns the closest nodes to send
     *              until an invalid value is returned. There should
     *              always be at least one entry in the yield object.
     * @throws MessageException in case the yield object doesn't contain
     *         items.
     */
    static const utils::Buffer make(
        const utils::Buffer& transactionID,
        const utils::Buffer& token,
        const dht::NodeData& source,
        const std::function<boost::optional<std::shared_ptr<Node> >()> yield);

    static const utils::Buffer make(
        const utils::Buffer& transactionID,
        const utils::Buffer& token,
        const dht::NodeData& source,
        const std::function<boost::optional<udp::endpoint> ()> yield);

    //! returns the parsed nodes
    boost::optional<std::vector<dht::NodeSPtr> > getNodes() const;

    //! return endpoints
    boost::optional<std::list< udp::endpoint > > getPeers() const;

    GetPeers& operator=( GetPeers&& ) = default;
    GetPeers& operator=( const GetPeers& ) = default;

private:

    void check() const;
};

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */
