#pragma once

#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/noncopyable.hpp>

namespace torrentsync
{
namespace dht
{

class RoutingTable : public boost::noncopyable
{
public:
    RoutingTable();
    ~RoutingTable();

private:


    //! Serialization friend class
    friend class boost::serialization::access;
    
    //! Split the serialization in 2 different steps
    BOOST_SERIALIZATION_SPLIT_MEMBER();

    template <class Archive>
    void save( Archive &ar, const unsigned int version) const;

    template <class Archive>
    void load( Archive &ar, const unsigned int version);
};

template <class Archive>
void RoutingTable::save( Archive &ar, const unsigned int version) const
{
    // TODO
}

template <class Archive>
void RoutingTable::load( Archive &ar, const unsigned int version)
{
    // TODO 
    // refresh all the nodes
    // 1. ping all known and insert them in the routing table with the normal procedure
    // 2. perform normal startup operation and let the bucket refreshing do it's job
}


}; // dht
}; // torrentsync

BOOST_CLASS_VERSION(torrentsync::dht::RoutingTable, 0);

