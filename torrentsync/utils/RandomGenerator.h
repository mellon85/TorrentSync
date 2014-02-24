#pragma once

#include <boost/scoped_ptr.hpp>

#include <cstdint>

namespace torrentsync
{
namespace utils
{

class RandomGenerator
{
public:

    static RandomGenerator& getInstance();

    virtual uint32_t get()=0;

protected:
    RandomGenerator();

private:
    static boost::scoped_ptr<RandomGenerator> generator;

};

}; // utils
}; // torrentsync
