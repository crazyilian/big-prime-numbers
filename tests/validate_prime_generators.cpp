#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "prime_generators/iterative_search.hpp"
#include "primality_tests/miller_rabin_test.hpp"

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
            EXPECT_TRUE(y == p);
        }
    }
}


}
