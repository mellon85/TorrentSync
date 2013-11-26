
#pragma once

#include <torrentsync/dht/AddressData.h>
#include <torrentsync/dht/Distance.h>
#include <boost/shared_ptr.hpp>

namespace torrentsync
{
namespace dht
{

class Address : public AddressData
{
public:
    Address( const std::string& );

    ~Address() {};

    // distance operator
    inline Distance operator^( const Address& addr ) const
    {
        Distance ret;
        ret.p1 = p1 ^ addr.p1;
        ret.p2 = p2 ^ addr.p2;
        ret.p3 = p3 ^ addr.p3;
        return ret;
    }

    inline void setGood()        { last_time_good = time(0); last_unanswered_queries = 0; }
    inline bool isGood()         const { return last_time_good > time(0)-good_interval; }
    inline bool isQuestionable() const { return !isGood() && last_unanswered_queries <= allowed_unanswered_queries;}
    inline bool isBad()          const { return !isGood() && last_unanswered_queries >  allowed_unanswered_queries;}
    inline const time_t& getLastTimeGood() const { return last_time_good; }

    static const time_t good_interval; 

    static const time_t allowed_unanswered_queries;

protected:
    inline Address() {};

    //! the last time the node was set as good
    time_t last_time_good;

    //! Number of the last unanswered queries
    size_t last_unanswered_queries;
};

typedef boost::shared_ptr<Address> AddressSPtr;

}; // dht
}; // torrentsync

namespace std
{
template <>
struct less<boost::shared_ptr<torrentsync::dht::Address> > :
    public std::binary_function<
        boost::shared_ptr<torrentsync::dht::Address>,
        boost::shared_ptr<torrentsync::dht::Address>, bool>
{
      bool operator()
          (const boost::shared_ptr<torrentsync::dht::Address>& x
          ,const boost::shared_ptr<torrentsync::dht::Address>& y ) const
          {return *x < *y;}
};
}; // std
 
