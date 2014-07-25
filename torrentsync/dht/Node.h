#pragma once

#include <boost/asio.hpp>

#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Distance.h>

#include <boost/shared_ptr.hpp>

namespace torrentsync
{
namespace dht
{

using boost::asio::ip::udp;

//! A DHT address with the associated statistics and informations
class Node : public NodeData
{
public:
    //! Copy constructor
    Node( const Node& addr );

    Node(
        const torrentsync::utils::Buffer&,
        const boost::optional<udp::endpoint>& = boost::optional<udp::endpoint>() );

    ~Node() {};

    static Node parse( const std::string& string );

    // distance operator
    Distance operator^( const Node& addr ) const noexcept;

    //! marks the address as good/fresh
    void setGood() noexcept;

    //! Is the address good/fresh?
    //! @return true if good/fresh
    bool isGood() const noexcept;

    bool isQuestionable()           const noexcept;
    bool isBad()                    const noexcept;
    const time_t& getLastTimeGood() const noexcept;

    static const time_t good_interval; 

    //! Maximum number of unanswered queries
    static const size_t allowed_unanswered_queries;

    //! returns the Peer's endpoint
    const boost::optional<udp::endpoint>& getEndpoint() const noexcept;

    void setEndpoint( udp::endpoint& );

    //! Parses a node information from the Buffer.
    //! In this class it implements the parsing of the actual ip address.
    //! @param begin the beginning of the data in the iterator
    //! @param end the beginning of the data in the iterator
    //! @throws std::invalid_argument in case the data is not correct
    void read(
        torrentsync::utils::Buffer::const_iterator begin,
        torrentsync::utils::Buffer::const_iterator end);

protected:
    Node() {};

    //! the last time the node was set as good
    time_t _last_time_good;

    //! Number of the last unanswered queries
    size_t _last_unanswered_queries;

    //! the endpoint of the node
    boost::optional<udp::endpoint> _endpoint;
};

typedef boost::shared_ptr<Node> NodeSPtr;

}; // dht
}; // torrentsync

namespace std
{
template <>
struct less<boost::shared_ptr<torrentsync::dht::Node> > :
    public std::binary_function<
        boost::shared_ptr<torrentsync::dht::Node>,
        boost::shared_ptr<torrentsync::dht::Node>, bool>
{
      bool operator()
          (const boost::shared_ptr<torrentsync::dht::Node>& x
          ,const boost::shared_ptr<torrentsync::dht::Node>& y ) const
          {return *x < *y;}
};
}; // std
 
