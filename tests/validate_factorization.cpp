#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "random.hpp"
#include "factorization/trial_factorization.h"
#include "primality_tests/miller_rabin_test.hpp"

namespace BigPrimeLib {

void check_factorization(const BigInt &n, std::vector<BigInt> &factors, PrimeTester &t) {
    BigInt x = 1;
    Random rnd;
    for (auto &p : factors) {
        auto status = t.test(p);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Factorization of " << n << " contains "
                << to_string(status) << ' ' << p;
        x *= p;
    }
    EXPECT_TRUE(x == n) << "Factorization of " << n << " is equal to " << x;
}

template<class FactorizationFunction, class... FactorizationArgs>
void validate_random_medium(PrimeTester &t, FactorizationFunction &factorization, FactorizationArgs &&... args) {
    Random rnd;
    BigInt C = Math::pow(BigInt(10), 12);
    for (size_t i = 0; i < 100; ++i) {
        auto n = rnd.uniform(1, C);
        auto factors = factorization(n, std::forward<FactorizationArgs>(args)...);
        check_factorization(n, factors, t);
    }
}

template<class FactorizationFunction, class... FactorizationArgs>
void validate_small(PrimeTester &t, FactorizationFunction &factorization, FactorizationArgs &&... args) {
    for (BigInt n = 1; n < 10000; ++n) {
        auto factors = factorization(n, std::forward<FactorizationArgs>(args)...);
        check_factorization(n, factors, t);
    }
}

template<class FactorizationFunction, class... FactorizationArgs>
void validate_small_divisors(PrimeTester &t, FactorizationFunction &factorization, FactorizationArgs &&... args) {
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
        check_factorization(n, factors, t);
    }
}

// trial_factorization

TEST(trial_factorization, small) {
    MillerRabinPrimeTester mrt(20, Random());
    validate_small(mrt, trial_factorization);
}

TEST(trial_factorization, random_medium) {
    MillerRabinPrimeTester mrt(20, Random());
    validate_random_medium(mrt, trial_factorization);
}

TEST(trial_factorization, validate_small_divisors) {
    MillerRabinPrimeTester mrt(20, Random());
    validate_small_divisors(mrt, trial_factorization);
}

}
