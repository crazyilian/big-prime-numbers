#pragma once

#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "primality_tests/primality_utils.h"

namespace BigPrimeLib {

static constexpr auto BIG_PRIMES_FILENAME = "data/random_big_primes.txt";

template<class PrimalityTestFunction, class... PrimalityTestArgs>
void validate_on_big_primes(const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
    std::vector<BigInt> big_primes = read_numbers(BIG_PRIMES_FILENAME);

    for (const BigInt &x : big_primes) {
        PrimalityStatus out = prime_test(x, std::forward<PrimalityTestArgs>(args)...);
        EXPECT_TRUE(out == PrimalityStatus::Prime || out == PrimalityStatus::Uncertain)
                << "Prime " << x << " marked as " << to_string(out);
    }
}

template<class PrimalityTestFunction, class... PrimalityTestArgs>
void validate_on_big_product_two_primes(const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
    std::vector<BigInt> big_primes = read_numbers(BIG_PRIMES_FILENAME);

    for (size_t i = 0; i < big_primes.size(); ++i) {
        for (size_t j = i; j < big_primes.size(); ++j) {
            BigInt x = big_primes[i] * big_primes[j];
            PrimalityStatus out = prime_test(x, std::forward<PrimalityTestArgs>(args)...);
            EXPECT_TRUE(out == PrimalityStatus::Composite)
                << "Composite " << big_primes[i] << "*" << big_primes[j] << " marked as " << to_string(out);
        }
    }
}

}
