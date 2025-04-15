#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "random.hpp"
#include "factorization/trial_factorization.h"
#include "primality_tests/miller_rabin_test.hpp"

namespace BigPrimeLib {

void check_factorization(const BigInt &n, std::vector<BigInt> &factors) {
    BigInt x = 1;
    Random rnd;
    for (auto &p : factors) {
        auto status = miller_rabin_prime_test_assume_prime(p, 20, rnd);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Factorization of " << n << " contains "
                << to_string(status) << ' ' << p;
        x *= p;
    }
    EXPECT_TRUE(x == n) << "Factorization of " << n << " is equal to " << x;
}

template<class FactorizationFunction, class... FactorizationArgs>
void validate_random_medium(FactorizationFunction &factorization, FactorizationArgs &&... args) {
    Random rnd;
    BigInt C = Math::pow(BigInt(10), 12);
    for (size_t i = 0; i < 100; ++i) {
        auto n = rnd.uniform(1, C);
        auto factors = factorization(n, std::forward<FactorizationArgs>(args)...);
        check_factorization(n, factors);
    }
}

template<class FactorizationFunction, class... FactorizationArgs>
void validate_small(FactorizationFunction &factorization, FactorizationArgs &&... args) {
    for (BigInt n = 1; n < 10000; ++n) {
        auto factors = factorization(n, std::forward<FactorizationArgs>(args)...);
        check_factorization(n, factors);
    }
}

template<class FactorizationFunction, class... FactorizationArgs>
void validate_small_divisors(FactorizationFunction &factorization, FactorizationArgs &&... args) {
    auto small_primes = read_numbers(Filenames::SmallPrimes);
    small_primes.resize(std::min(small_primes.size(), 500ul));
    Random rnd;
    for (size_t cnt_div = 10; cnt_div <= 200; cnt_div += 10) {
        BigInt n = 1;
        for (size_t i = 0; i < cnt_div; ++i) {
            size_t ind = static_cast<size_t>(rnd.uniform(0, small_primes.size() - 1));
            n *= small_primes[ind];
        }
        auto factors = factorization(n, std::forward<FactorizationArgs>(args)...);
        check_factorization(n, factors);
    }
}

// trial_factorization

TEST(trial_factorization, small) {
    validate_small(trial_factorization);
}

TEST(trial_factorization, random_medium) {
    validate_random_medium(trial_factorization);
}

TEST(trial_factorization, validate_small_divisors) {
    validate_small_divisors(trial_factorization);
}

}
