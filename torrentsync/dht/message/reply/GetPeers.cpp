#include <torrentsync/dht/message/BEncodeEncoder.h>
#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Node.h>
#include <torrentsync/dht/message/reply/GetPeers.h>
#include <torrentsync/utils/Buffer.h>
#include <torrentsync/dht/DHTConstants.h>

#include <torrentsync/utils/Yield.h>
#include <boost/lexical_cast.hpp>

namespace torrentsync
{
namespace dht
{
namespace message
{
namespace reply
{
using namespace torrentsync;

const utils::Buffer NODES_FIELD  = Field::Reply + Field::Separator+ Field::Nodes;
const utils::Buffer VALUES_FIELD = Field::Reply + Field::Separator + Field::Values;

GetPeers::GetPeers(const DataMap& dataMap) : dht::message::Reply(dataMap)
{
    check();
}

const utils::Buffer GetPeers::make(
    const utils::Buffer& transactionID,
    const utils::Buffer& token,
    const dht::NodeData& source,
    const std::function<boost::optional<dht::NodeSPtr> ()> yield)
{
    utils::Buffer nodeData(PACKED_NODE_SIZE*DHT_FIND_NODE_COUNT);

    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Reply);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());

    utils::Buffer nodes_list;
    nodes_list.reserve( PACKED_NODE_SIZE * MAX_NODES_PER_GETPEERS);
    utils::for_each(yield,[&](const dht::NodeSPtr& node)
    {
        nodes_list += node->getPackedNode();
    });

    if (nodes_list.size() == 0)
        throw dht::message::MessageException(
            "GetPeers message has no nodes",
            dht::message::ErrorType::serverError);

    enc.addDictionaryElement(
        torrentsync::dht::message::Field::Nodes,nodes_list);

    enc.addDictionaryElement(
        torrentsync::dht::message::Field::Token, token);

    enc.endDictionary();
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Reply);
    enc.endDictionary();
    return enc.value();
}

const utils::Buffer GetPeers::make(
    const utils::Buffer& transactionID,
    const utils::Buffer& token,
    const dht::NodeData& source,
    const std::function<boost::optional<udp::endpoint> ()> yield)
{
    utils::Buffer nodeData(PACKED_NODE_SIZE*DHT_FIND_NODE_COUNT);

    BEncodeEncoder enc;
    enc.startDictionary();
    enc.addElement(Field::Reply);
    enc.startDictionary();
    enc.addDictionaryElement(Field::PeerID,source.write());

    enc.addDictionaryElement(
        torrentsync::dht::message::Field::Token, token);

    enc.addElement(
        torrentsync::dht::message::Field::Values);
    enc.startList();

    size_t elementCount = 0;
    utils::for_each(yield,[&](const udp::endpoint& endpoint)
    {
        auto peer_buffer = dht::packEndpoint(endpoint);
        enc.addElement(peer_buffer);
        ++elementCount;
    });

    if (elementCount == 0)
        throw dht::message::MessageException(
            "GetPeers message has no peers",
            dht::message::ErrorType::serverError);

    enc.endList();

    enc.endDictionary();
    enc.addDictionaryElement(Field::TransactionID,transactionID);
    enc.addDictionaryElement(Field::Type,Type::Reply);
    enc.endDictionary();
    return enc.value();
}

boost::optional<std::vector<dht::NodeSPtr> > GetPeers::getNodes() const
{
    auto token = find(NODES_FIELD);
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

boost::optional<std::list<udp::endpoint>> GetPeers::getPeers() const
{
    size_t index = 0;
    auto token = find(VALUES_FIELD+"/"+
            boost::lexical_cast<std::string>(index));
    if(!token)
        return boost::optional<std::list< udp::endpoint> >();

    std::list<udp::endpoint> peers;

    do
    {
        peers.push_back(unpackEndpoint(token->begin()));
        token = find(VALUES_FIELD+"/"+
            boost::lexical_cast<std::string>(++index));
    } while (!!token);

    return peers;
}

void GetPeers::check() const
{
    if (!find(Field::Reply + "/" + Field::PeerID))
        throw MessageException(
            "Missing reply peer in find_node reply",
            ErrorType::protocolError);
    const bool foundNodes = !!find(NODES_FIELD);
    const bool foundPeers = !!find(VALUES_FIELD+"/0");
    if ( !foundNodes && !foundPeers )
        throw MessageException(
            "Missing nodes/peers in find_node reply",
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

