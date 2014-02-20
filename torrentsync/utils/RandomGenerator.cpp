#include <ctime>

#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

#include <torrentsync/utils/RandomGenerator.h>
#include <torrentsync/utils/Lock.h>

namespace torrentsync
{
namespace utils
{

class RandomGeneratorImpl : public RandomGenerator
{
public:
    RandomGeneratorImpl() : dist(0)
    {
        gen.seed(static_cast<unsigned int>(std::time(0)));
    }

    virtual uint32_t get()
    {
        return dist(gen);
    }

private:
    boost::random::mt19937 gen;
    boost::random::uniform_int_distribution<uint32_t> dist;
};

boost::scoped_ptr<RandomGenerator> RandomGenerator::generator;

RandomGenerator& RandomGenerator::getInstance()
{
    static Mutex mutex;      
    WriteLock lock(mutex);
    if (!generator.get())
    {
        generator.reset(new RandomGeneratorImpl());
    }
    return *generator;
}

RandomGenerator::RandomGenerator()
{
}

}; // utils
}; // torrentsync
