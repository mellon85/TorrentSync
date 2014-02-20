#pragma once

#include <boost/scoped_ptr.hpp>

#ifdef HAS_Cxx11
#include <cstdint>
#else
#include <stdint.h>
#endif

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
