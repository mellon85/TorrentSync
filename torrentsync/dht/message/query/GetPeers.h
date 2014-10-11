#pragma once

#include <torrentsync/dht/message/Query.h>
#include <torrentsync/utils/Buffer.h>


namespace torrentsync
{
namespace dht
{

class NodeData;

namespace message
{
namespace query
{

//! Abstract class representing every message
class GetPeers : public dht::message::Query
{
public:
    //! GetPeers constructor to initialize the class from a raw data map
    GetPeers(const DataMap& dataMap);

    GetPeers( GetPeers&& ) = default;

    GetPeers& operator=( GetPeers&& ) = default;

    //! Destructor
    virtual ~GetPeers() {}

    //! creates a GetPeers message
    //! @param transactionID the ID
    //! @param infoHash looking for
    //! @param address (should be our own address)
    static const utils::Buffer make( 
        const utils::Buffer& transactionID,
        const utils::Buffer& infoHash,
        const dht::NodeData& address);

    //! returns the info_hash contained in the message
    utils::Buffer getInfoHash() const;

};
 
} /* query */
} /* message */
} /* dht */
} /* torrentsync */
