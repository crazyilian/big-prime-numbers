#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "prime_generators/iterative_search.hpp"
#include "primality_tests/miller_rabin_test.hpp"
#include "primality_tests/bpsw_test.h"

namespace BigPrimeLib {

// search_next_prime

TEST(search_next_prime, small_primes) {
    Random rnd;
    auto small_primes = read_numbers(Filenames::SmallPrimes);
    small_primes.resize(std::min(small_primes.size(), 1000ul));
    BigInt x = -10;
    for (const BigInt &p : small_primes) {
        for (; x <= p; ++x) {
            auto y = search_next_prime(x, miller_rabin_prime_test_assume_prime<decltype(rnd)>, 10, rnd);
            EXPECT_TRUE(y == p) << "First prime after " << x << " is " << p << ", but found " << y;
        }
    }
}

// generate_random_prime_in_range

TEST(generate_random_prime_in_range, small_range) {
    Random rnd;
    auto small_primes = read_numbers(Filenames::SmallPrimes);
    small_primes.resize(std::min(small_primes.size(), 3000ul));
    for (const BigInt &p : small_primes) {
        BigInt l = p - rnd.uniform(0, 100);
        BigInt r = p + rnd.uniform(0, 100);
        auto y = generate_random_prime_in_range(l, r, rnd,
                                                miller_rabin_prime_test_assume_prime<decltype(rnd)>, 10, rnd);
        EXPECT_TRUE(l <= y && y <= r) << y << " is not in range " << "[" << l << ", " << r << "]";
        auto status = miller_rabin_prime_test_assume_prime(y, 10, rnd);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found not " << to_string(status) << " number " << y;
    }
}

TEST(generate_random_prime_in_range, big_range) {
    Random rnd;
    Random rnd_test;
    BigInt C = Math::pow(BigInt(10), 300);
    for (size_t i = 0; i < 10; ++i) {
        BigInt l = rnd_test.uniform(0, C);
        BigInt r = rnd_test.uniform(2 * l, 4 * C);
        auto y = generate_random_prime_in_range(l, r, rnd,
                                                bpsw_fermat_prime_test_assume_prime, true, false);
        EXPECT_TRUE(l <= y && y <= r) << y << " is not in range " << "[" << l << ", " << r << "]";
        auto status = miller_rabin_prime_test_assume_prime(y, 20, rnd_test);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found " << to_string(status) << " number " << y;
    }
}


}
