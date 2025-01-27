#pragma once

#include <gtest/gtest.h>
#include "common.hpp"
#include "test_utils.h"
#include "primality_tests/primality_status.h"

namespace BigPrimeLib {

static constexpr auto SMALL_PRIMES_FILENAME = "first_10000_primes.txt";

template<class PrimalityTestFunction, class... PrimalityTestArgs>
void validate_on_small_primes(const std::set<BigInt> &skip, const PrimalityTestFunction &prime_test,
                              PrimalityTestArgs &&... args) {
    std::vector<BigInt> small_primes = read_numbers(SMALL_PRIMES_FILENAME);
    std::set<BigInt> small_primes_set(small_primes.begin(), small_primes.end());

    for (BigInt x = 2; x <= small_primes.back(); ++x) {
        if (skip.contains(x)) {
            continue;
        }
        bool is_prime = small_primes_set.contains(x);
        PrimalityStatus out = prime_test(x, std::forward<PrimalityTestArgs>(args)...);
        if (is_prime) {
            EXPECT_TRUE(out == PrimalityStatus::Prime || out == PrimalityStatus::Uncertain)
                    << "Prime " << x << " marked as " << to_string(out);
        } else {
            EXPECT_TRUE(out == PrimalityStatus::Composite)
                    << "Composite " << x << " marked as " << to_string(out);
        }
    }
}

}
