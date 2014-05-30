
#pragma once

#include <torrentsync/dht/NodeData.h>
#include <torrentsync/dht/Distance.h>

#include <boost/shared_ptr.hpp>

namespace torrentsync
{
namespace dht
{

//! A DHT address with the associated statistics and informations
class Node : public NodeData
{
public:
    //! Builds an address from an hexadecimal string
    Node( const std::string& );

    //! Copy constructor
    Node( const Node& addr );

    inline ~Node() {};

    // distance operator
    inline Distance operator^( const Node& addr ) const;

    //! marks the address as good/fresh
    inline void setGood();

    //! Is the address good/fresh?
    //! @return true if good/fresh
    inline bool isGood() const;

    inline bool isQuestionable() const;
    inline bool isBad()          const;
    inline const time_t& getLastTimeGood() const;

    static const time_t good_interval; 

    static const size_t allowed_unanswered_queries;

protected:
    Node() {};

    //! the last time the node was set as good
    time_t last_time_good;

    //! Number of the last unanswered queries
    size_t last_unanswered_queries;
};

typedef boost::shared_ptr<Node> NodeSPtr;

void Node::setGood() 
{
    last_time_good = time(0);
    last_unanswered_queries = 0;
}

bool Node::isGood() const
{
    return last_time_good > time(0)-good_interval;
}

bool Node::isQuestionable() const {
    return !isGood() && last_unanswered_queries <= allowed_unanswered_queries;
}

bool Node::isBad() const {
    return !isGood() && last_unanswered_queries >  allowed_unanswered_queries;
}

const time_t& Node::getLastTimeGood() const
{
    return last_time_good;
}

Distance Node::operator^( const Node& addr ) const
{
    Distance ret;
    ret.p1 = p1 ^ addr.p1;
    ret.p2 = p2 ^ addr.p2;
    ret.p3 = p3 ^ addr.p3;
    return ret;
}


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
 
