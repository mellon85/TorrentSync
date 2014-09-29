#include <atomic>
#include <chrono>
#include <mutex>

#include <torrentsync/dht/TokenManager.h>
#include <torrentsync/utils/RandomGenerator.h>
#include <torrentsync/utils/Spinlock.h>

#include <openssl/evp.h>

namespace torrentsync
{
namespace dht
{

namespace TokenManager
{

namespace utils = torrentsync::utils;
using boost::asio::ip::udp;

typedef std::array<uint32_t, 3> seeds_t;
typedef std::chrono::steady_clock spinlock_clock_t;
typedef std::chrono::time_point<spinlock_clock_t> spinlock_time_point_t;

static seeds_t   _random_seeds;
static std::chrono::time_point<spinlock_clock_t> last_time;

static utils::Spinlock spinlock;

seeds_t getAndUpdateSeeds()
{
    const spinlock_time_point_t now      = spinlock_clock_t::now();
    const spinlock_time_point_t interval = last_time + std::chrono::minutes(5);;

    std::lock_guard<utils::Spinlock> lock(spinlock);

    if (now > interval)
    {
        for( int _i = _random_seeds.max_size()-1; _i > 0; --_i )
        {
            _random_seeds[_i-1] = _random_seeds[_i];
        }
        _random_seeds[0] = utils::RandomGenerator::getInstance().get();
    }

    return _random_seeds;
}

utils::Buffer calculateToken(
    const udp::endpoint& endpoint,
    seeds_t::value_type seed) 
{
    auto port = endpoint.port();

    EVP_MD_CTX ctx;
    EVP_MD_CTX_init(&ctx);

    if (!EVP_DigestInit_ex(&ctx,EVP_sha1(),NULL))
        throw std::runtime_error("OpenSSL Hash Init Error");

    if (!EVP_DigestUpdate(&ctx, &seed, sizeof(seed)) ||
        !EVP_DigestUpdate(&ctx, &port, sizeof(port)))
        throw std::runtime_error("OpenSSL Hash data Error");

    if (endpoint.address().is_v4())
    {
        const auto data = endpoint.address().to_v4().to_bytes();
        if (!EVP_DigestUpdate(&ctx, &data, sizeof(data)))
            throw std::runtime_error("OpenSSL Hash address Error");
    }
    else
    {
        assert(endpoint.address().is_v6());
        const auto data = endpoint.address().to_v6().to_bytes();
        if (!EVP_DigestUpdate(&ctx, &data, sizeof(data)))
            throw std::runtime_error("OpenSSL Hash address Error");
    }

    uint8_t  hash[EVP_MAX_MD_SIZE];
    uint32_t hash_length;
    if (!EVP_DigestFinal_ex(&ctx, hash, &hash_length))
        throw std::runtime_error("OpenSSL Hash address Error");

    EVP_MD_CTX_destroy(&ctx);

    return utils::makeBuffer( hash, hash_length );

}

utils::Buffer getToken( const udp::endpoint& endpoint )
{
    return calculateToken(
            endpoint,
            getAndUpdateSeeds()[0]);
}

bool isTokenValid(
    const utils::Buffer& token,
    const udp::endpoint& endpoint)
{
    const seeds_t seeds = getAndUpdateSeeds();
    bool ret = false;
    std::for_each( seeds.begin(), seeds.end(), [&](seeds_t::value_type seed){
        if (calculateToken(endpoint,seed) == token)
        {
            ret = true;
            return;
        }
    });
    return ret;
}

} /* TokenManager */ 

} /* dht */ 
} /* torrentsync */

