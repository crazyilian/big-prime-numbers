#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "random.hpp"
#include "factorization/trial_factorization.h"
#include "primality_tests/miller_rabin_test.hpp"

namespace BigPrimeLib {

// utils

std::vector<BigInt> read_all_big_primes() {
    auto big_primes = read_numbers(Filenames::BigPrimes);
    auto big_generalized_mersenne_primes = read_numbers(Filenames::BigGeneralizedMersennePrimes);
    auto big_proth_primes = read_numbers(Filenames::BigProthPrimes);
    for (size_t i = 0; i < big_generalized_mersenne_primes.size(); i += 2) {
        auto n = static_cast<uint64_t>(big_generalized_mersenne_primes[i]);
        auto k = big_generalized_mersenne_primes[i + 1];
        big_primes.push_back((k << n) - 1);
    }
    for (size_t i = 0; i < big_proth_primes.size(); i += 2) {
        auto n = static_cast<uint64_t>(big_proth_primes[i]);
        auto k = big_proth_primes[i + 1];
        big_primes.push_back((k << n) + 1);
    }
    std::sort(big_primes.begin(), big_primes.end());
    return big_primes;
}

void check_factorization(const BigInt &n, const std::vector<BigInt> &factors, PrimeTester &t) {
    BigInt x = 1;
    for (auto &p : factors) {
        auto status = t.test(p);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Factorization of " << n << " contains "
                << to_string(status) << ' ' << p;
        x *= p;
    }
    EXPECT_TRUE(x == n) << "Factorization of " << n << " is equal to " << x;
}

// gen numbers

BigInt gen_small_divisors(const BigInt &max_divisor, size_t cnt, Random<> &rnd) {
    static auto primes = read_numbers(Filenames::SmallPrimes);
    std::vector divisors(primes.begin(), std::upper_bound(primes.begin(), primes.end(), max_divisor));
    BigInt n = 1;
    for (size_t i = 0; i < cnt; ++i) {
        size_t ind = static_cast<size_t>(rnd.uniform(0, divisors.size() - 1));
        n *= divisors[ind];
    }
    return n;
}

BigInt gen_big_prime(const BigInt &max_val, Random<> &rnd) {
    static auto primes = read_all_big_primes();
    std::vector divisors(primes.begin(), std::upper_bound(primes.begin(), primes.end(), max_val));
    size_t ind = static_cast<size_t>(rnd.uniform(0, divisors.size() - 1));
    return divisors[ind];
}

BigInt gen_big_divisors(const BigInt &max_divisor, size_t cnt, Random<> &rnd) {
    BigInt n = 1;
    for (size_t i = 0; i < cnt; ++i) {
        n *= gen_big_prime(max_divisor, rnd);
    }
    return n;
}

// validate

void validate_small(PrimeTester &t, Factorizer &f) {
    for (BigInt n = 1; n < 10000; ++n) {
        check_factorization(n, f.factorization(n), t);
    }
}

void validate_medium(PrimeTester &t, Factorizer &f) {
    Random rnd;
    for (size_t i = 0; i < 100; ++i) {
        auto n = rnd.uniform(1, BigInt(1e12));
        check_factorization(n, f.factorization(n), t);
    }
}

void validate_many_small(PrimeTester &t, Factorizer &f) {
    Random rnd;
    for (size_t cnt_div = 10; cnt_div <= 200; cnt_div += 10) {
        BigInt n = gen_small_divisors(4000, cnt_div, rnd);
        check_factorization(n, f.factorization(n), t);
    }
}

void validate_many_small_one_big(PrimeTester &t, Factorizer &f) {
    Random rnd;
    for (size_t cnt_div = 10; cnt_div <= 100; cnt_div += 10) {
        BigInt n = gen_small_divisors(2000, cnt_div, rnd);
        n *= gen_big_prime(BigInt(1e1000l), rnd);
        check_factorization(n, f.factorization(n), t);
    }
}

void validate_many_small_two_big(PrimeTester &t, Factorizer &f) {
    Random rnd;
    for (size_t cnt_div = 10; cnt_div <= 100; cnt_div += 10) {
        BigInt n = gen_small_divisors(2000, cnt_div, rnd);
        BigInt p1 = gen_big_prime(BigInt(1e700l), rnd);
        BigInt p2 = gen_big_prime(BigInt(1e700l), rnd);
        n *= p1 * p2;
        check_factorization(n, f.factorization(n), t);
    }
}

void validate_several_big(PrimeTester &t, Factorizer &f) {
    Random rnd;
    for (size_t cnt = 1; cnt <= 5; ++cnt) {
        BigInt max_div = Math::pow(BigInt(10), 5000 / cnt);
        for (size_t i = 0; i < 10; ++i) {
            BigInt n = gen_big_divisors(max_div, cnt, rnd);
            check_factorization(n, f.factorization(n), t);
        }
    }
}

// trial_factorization

TEST(trial_factorization, small_and_medium) {
    MillerRabinPrimeTester mrt(20, Random());
    TrialFactorizer f;
    validate_small(mrt, f);
    validate_medium(mrt, f);
}


TEST(trial_factorization, many_small) {
    MillerRabinPrimeTester mrt(20, Random());
    TrialFactorizer f;
    validate_many_small(mrt, f);
}

TEST(trial_factorization_with_prime_test, many_small_one_big) {
    MillerRabinPrimeTester mrt(20, Random());
    TrialFactorizer f(MillerRabinPrimeTester(10, Random()));
    validate_many_small_one_big(mrt, f);
}

}
