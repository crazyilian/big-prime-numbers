#include <gtest/gtest.h>
#include "common.h"
#include "small_validator.hpp"
#include "big_validator.hpp"
#include "primality_tests/trial_test.h"
#include "primality_tests/fermat_test.hpp"
#include "primality_tests/miller_rabin_prime_test.hpp"

namespace BigPrimeLib {

TEST(trial_prime_test, small_primes) {
    validate_on_small_primes({}, trial_prime_test);
}

TEST(fermat_prime_test, small_primes) {
    Random rnd;

    std::unordered_set<BigInt> carmichael = {
        561, 1105, 1729, 2465, 2821, 6601, 8911, 10585, 15841, 29341, 41041, 46657, 52633, 62745, 63973, 75361, 101101
    };
    validate_on_small_primes(carmichael, fermat_prime_test<DefaultRandomGenerator>, 10, rnd);
}

TEST(fermat_prime_test, big_primes) {
    Random rnd;
    validate_on_big_primes(fermat_prime_test<DefaultRandomGenerator>, 10, rnd);
}

TEST(fermat_prime_test, big_product_two_primes) {
    Random rnd;
    validate_on_big_product_two_primes(fermat_prime_test<DefaultRandomGenerator>, 10, rnd);
}

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

}
