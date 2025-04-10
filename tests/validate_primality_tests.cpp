#include <gtest/gtest.h>
#include "common.h"
#include "small_validator.hpp"
#include "big_validator.hpp"
#include "primality_tests/trial_test.h"
#include "primality_tests/fermat_test.hpp"
#include "primality_tests/miller_rabin_prime_test.hpp"

namespace BigPrimeLib {

constexpr auto kSmallCarmichaelFilename = "data/carmichael_less_10000th_prime.txt";

// trial_prime_test

TEST(trial_prime_test, small_primes) {
    validate_on_small_primes({}, trial_prime_test);
}

// fermat_prime_test

TEST(fermat_prime_test, small_primes) {
    Random rnd;
    auto carmichael = read_numbers(kSmallCarmichaelFilename);
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
    auto carmichael = read_numbers(kSmallCarmichaelFilename);
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

}
