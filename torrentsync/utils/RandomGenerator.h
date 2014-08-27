#pragma once

#include <memory>
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
    static std::unique_ptr<RandomGenerator> generator;

};

}; // utils
}; // torrentsync
