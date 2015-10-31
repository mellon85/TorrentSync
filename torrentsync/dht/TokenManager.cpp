#include <atomic>
#include <chrono>
#include <mutex>

#include <torrentsync/dht/TokenManager.h>
#include <torrentsync/utils/RandomGenerator.h>
#include <torrentsync/utils/log/Logger.h>
#include <boost/range/irange.hpp>
#include <cassert>
#include <algorithm>
#include <random>

// OpenSSL
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

seeds_t getAndUpdateSeeds()
{
    static std::uniform_int_distribution<uint32_t> dist;
    static const auto token_interval = std::chrono::minutes(5);

    static std::random_device random;
    static std::mutex mutexlock;
    static seeds_t  _random_seeds;
    static spinlock_time_point_t last_time;

    const auto now = spinlock_clock_t::now();

    std::lock_guard<std::mutex> lock(mutexlock);

    // check if more than one token has expired. something weird is going on if
    // there were no tokens generated in the last 5*x minutes. Reset all the
    // invalidated tokens, not just one shift.
    const size_t number_of_intervals = std::floor((now - last_time) / token_interval);
    if (number_of_intervals >= 1)
    {
        // shift by one to create space for a new one
        for( auto _i : boost::irange(
                    static_cast<int>(_random_seeds.max_size())-1, 1, -1) )
        {
            assert(_i >= 0 && _i < (int)_random_seeds.size());
            _random_seeds[_i-1] = _random_seeds[_i];
        }
        _random_seeds[0] = dist(random);

        // cover the additional old ones that have expired with the latest
        // shifted one.
        const size_t index_to_remove_from = number_of_intervals-1;
        std::fill(
                _random_seeds.end() - index_to_remove_from,
                _random_seeds.end(),
                _random_seeds[
                    std::max(0ul,_random_seeds.size() - number_of_intervals - 1)]);
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
    const udp::endpoint& endpoint) noexcept
{
    const seeds_t seeds = getAndUpdateSeeds();

    for( auto &seed : seeds )
    {
        try
        {
            if (calculateToken(endpoint,seed) == token)
            {
                return true;
            }
        }
        catch( const std::runtime_error& )
        {
            LOG(ERROR,"TokenManager * Failed to calculate a token for " << endpoint);
        }
    };
    return false;
}

} /* TokenManager */
} /* dht */
} /* torrentsync */
