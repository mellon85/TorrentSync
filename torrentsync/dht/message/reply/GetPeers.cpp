#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/reply/GetPeers.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>

#include <torrentsync/utils/Yield.h>

/*
TODO:

! Add to Node a token entry.
An 8 byte buffer key for the announce to return from get_peers.

*/

namespace torrentsync
{
namespace dht
{
namespace message
{
namespace reply
{

using namespace torrentsync;

GetPeers::GetPeers(const DataMap& dataMap) : dht::message::Reply(dataMap)
{
    check();
}

const utils::Buffer GetPeers::make( 
    const utils::Buffer& transactionID,
    const dht::NodeData& source,
    const std::function<boost::optional<dht::NodeSPtr> ()> nodes,
    const ParameterType nodesOrPeers)
{
    utils::Buffer nodeData(PACKED_NODE_SIZE*DHT_FIND_NODE_COUNT);

    size_t bufferIndex = 0;
    utils::for_each(nodes, [&](const dht::NodeSPtr& it)
    {
        if (nodesOrPeers == Nodes)
        {
            const utils::Buffer data = it->getPackedNode();
            assert(bufferIndex + data.size() <= nodeData.size());
            std::copy(data.cbegin(),data.cend(),nodeData.begin()+bufferIndex);
            bufferIndex += PACKED_NODE_SIZE;
        }
        else
        {
            assert(nodesOrPeers == Peers);
            const utils::Buffer data = it->getPackedPeer();
            assert(bufferIndex + data.size() <= nodeData.size());
            std::copy(data.cbegin(),data.cend(),nodeData.begin()+bufferIndex);
            bufferIndex += PACKED_PEER_SIZE;
        }
    });
    nodeData.resize(bufferIndex);

    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Reply);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());

    if (nodesOrPeers == Nodes)
    {
         // "nodes": "def456..." like in FindNode
    }

    // TODO token

    if (nodesOrPeers == Peers)
    {
        //  "values": ["axje.u", "idhtnm"]}}
    }

    enc.endDictionary();
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Reply); 
    enc.endDictionary();
    return enc.value();
}

boost::optional<std::vector<dht::NodeSPtr> > GetPeers::getNodes() const
{
    auto token = find( Field::Reply + "/" + Field::Nodes );
    if(!token)
        return boost::optional<std::vector<dht::NodeSPtr> >();

    std::vector<dht::NodeSPtr> nodes;
    const utils::Buffer& buff = *token;
    for( auto it = buff.begin(); it+PACKED_NODE_SIZE <= buff.end(); it += PACKED_NODE_SIZE )
    {
        nodes.push_back(NodeSPtr(new Node(it,it+PACKED_NODE_SIZE)));
    }

    return nodes;
}

boost::optional<std::vector<udp::endpoint>> GetPeers::getPeers() const
{
    auto token = find( Field::Reply + "/" + Field::Values );
    if(!token)
        return boost::optional<std::vector< udp::endpoint> >();

    std::vector<udp::endpoint> peers;

    // TODO parse peers

    return peers;
}

void GetPeers::check() const
{
    if (!find(Field::Reply + "/" + Field::PeerID))
        throw MessageException("Missing nodes in find_node reply",
                ErrorType::protocolError);
    if (!find(Field::Reply + "/" + Field::Nodes) && !find(Field::Reply + "/" + Field::Values))
        throw MessageException("Missing nodes in find_node reply",
                ErrorType::protocolError);
}

GetPeers::GetPeers( Message&& m ) : Reply(std::move(m))
{
    check();
}

GetPeers::GetPeers( const Message& m ) : Reply(m)
{
    check();
}

} /* reply */
} /* message */
} /* dht */
} /* torrentsync */

