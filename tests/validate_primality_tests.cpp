#include <gtest/gtest.h>
#include "common.h"
#include "small_validator.hpp"
#include "big_validator.hpp"
#include "primality_tests/trial_test.hpp"
#include "primality_tests/fermat_test.hpp"
#include "primality_tests/miller_rabin_test.hpp"
#include "primality_tests/lucas_lehmer_test.hpp"
#include "primality_tests/lucas_lehmer_riesel_test.hpp"

namespace BigPrimeLib {


// trial_prime_test

TEST(trial_prime_test, small_primes) {
    validate_on_small_primes({}, trial_prime_test);
}

// fermat_prime_test

TEST(fermat_prime_test, small_primes) {
    Random rnd;
    auto carmichael = read_numbers(Filenames::SmallCarmichael);
    std::unordered_set<BigInt> skip(carmichael.begin(), carmichael.end());
    validate_on_small_primes(skip, fermat_prime_test<DefaultRandomGenerator>, 10, rnd);
}

TEST(fermat_prime_test, big_primes) {
    Random rnd;
    validate_on_big_primes(fermat_prime_test<DefaultRandomGenerator>, 10, rnd);
}

TEST(fermat_prime_test, big_product_two_primes) {
    Random rnd;
    validate_on_big_product_two_primes(fermat_prime_test<DefaultRandomGenerator>, 10, rnd);
}

// fermat_prime_test_iter

TEST(fermat_prime_test_iter, small_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    auto carmichael = read_numbers(Filenames::SmallCarmichael);
    std::unordered_set<BigInt> skip(carmichael.begin(), carmichael.end());
    skip.insert(bases.begin(), bases.end());
    validate_on_small_primes(skip, fermat_prime_test_iter<std::vector<BigInt>::iterator>, bases.begin(), bases.end());
}

TEST(fermat_prime_test_iter, big_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_primes(fermat_prime_test_iter<std::vector<BigInt>::iterator>, bases.begin(), bases.end());
}

TEST(fermat_prime_test_iter, big_product_two_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_product_two_primes(fermat_prime_test_iter<std::vector<BigInt>::iterator>, bases.begin(),
                                       bases.end());
}

// miller_rabin_prime_test

TEST(miller_rabin_prime_test, small_primes) {
    Random rnd;
    validate_on_small_primes({}, miller_rabin_prime_test<DefaultRandomGenerator>, 10, rnd);
}

TEST(miller_rabin_prime_test, big_primes) {
    Random rnd;
    validate_on_big_primes(miller_rabin_prime_test<DefaultRandomGenerator>, 10, rnd);
}

TEST(miller_rabin_prime_test, big_product_two_primes) {
    Random rnd;
    validate_on_big_product_two_primes(miller_rabin_prime_test<DefaultRandomGenerator>, 10, rnd);
}

// miller_rabin_prime_test_iter

TEST(miller_rabin_prime_test_iter, small_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    std::unordered_set<BigInt> skip(bases.begin(), bases.end());
    skip.insert(12);
    validate_on_small_primes(skip, miller_rabin_prime_test_iter<std::vector<BigInt>::iterator>, bases.begin(),
                             bases.end());
}

TEST(miller_rabin_prime_test_iter, big_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_primes(miller_rabin_prime_test_iter<std::vector<BigInt>::iterator>, bases.begin(), bases.end());
}

TEST(miller_rabin_prime_test_iter, big_product_two_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_product_two_primes(miller_rabin_prime_test_iter<std::vector<BigInt>::iterator>, bases.begin(),
                                       bases.end());
}

// lucas_lehmer_prime_test

TEST(lucas_lehmer_prime_test, small_mersenne) {
    auto small_primes = read_numbers(Filenames::SmallPrimes);
    auto mersenne_primes = read_numbers(Filenames::MersennePrimes);
    std::set<BigInt> mersenne_primes_set(mersenne_primes.begin(), mersenne_primes.end());
    for (const BigInt &p : small_primes) {
        if (p > 3300) {
            break;
        }
        auto status = lucas_lehmer_prime_test(static_cast<uint64_t>(p));
        if (mersenne_primes_set.contains(p)) {
            EXPECT_TRUE(status == PrimalityStatus::Prime) << "Prime 2^" << p << "-1 marked as " << to_string(status);
        } else {
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Prime 2^" << p << "-1 marked as "
                    << to_string(status);
        }
    }
}

// lucas_lehmer_riesel_prime_test

TEST(lucas_lehmer_riesel_prime_test, small_generalized_mersenne) {
    for (uint64_t n = 1; n < 200; ++n) {
        for (uint64_t k = 1; k < 200 && (n >= 10 || k < (1 << n)); ++k) {
            // 2^n > k
            BigInt N = k * Math::pow(BigInt(2), n) - 1;
            auto status_lucas = lucas_lehmer_riesel_prime_test(k, n);
            Random rnd;
            auto status_miller = uncertain2prime(miller_rabin_prime_test(N, 20, rnd));
            EXPECT_TRUE(status_lucas == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                    << to_string(status_lucas);

        }
    }
}

TEST(lucas_lehmer_riesel_prime_test, big_generalized_mersenne_random) {
    Random rnd(42);
    for (size_t i = 0; i < 10; ++i) {
        auto n = static_cast<uint64_t>(rnd.uniform(1, 5000));
        auto k = rnd.uniform(1, Math::pow(BigInt(2), n) - 1);

        BigInt N = k * Math::pow(BigInt(2), n) - 1;
        auto status_lucas = lucas_lehmer_riesel_prime_test(k, n);
        auto status_miller = uncertain2prime(miller_rabin_prime_test(N, 20, rnd));
        EXPECT_TRUE(status_lucas == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                << to_string(status_lucas);
    }
}

TEST(lucas_lehmer_riesel_prime_test, big_generalized_mersenne_primes) {
    auto numbers = read_numbers(Filenames::BigGeneralizedMersennePrimes);
    for (size_t i = 0; i < numbers.size(); i += 2) {
        auto n = static_cast<uint64_t>(numbers[i]);
        auto k = numbers[i + 1];
        auto status = lucas_lehmer_riesel_prime_test(k, n);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Prime " << k << " * 2^" << n << " - 1 marked as "
                << to_string(status);
    }
}

}
