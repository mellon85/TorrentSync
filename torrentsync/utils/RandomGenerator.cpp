#include <ctime>
#include <random>
#include <mutex>

#include <torrentsync/utils/RandomGenerator.h>

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
    std::mt19937 gen;
    std::uniform_int_distribution<uint32_t> dist;
};

std::unique_ptr<RandomGenerator> RandomGenerator::generator;

RandomGenerator& RandomGenerator::getInstance()
{
    static std::mutex mutex;      
    std::lock_guard<std::mutex> lock(mutex);
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
