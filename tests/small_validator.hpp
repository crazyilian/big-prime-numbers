#pragma once

#include <unordered_set>
#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "primality_tests/primality_utils.h"

namespace BigPrimeLib {

template<class PrimalityTestFunction, class... PrimalityTestArgs>
void validate_on_small_primes(const std::unordered_set<BigInt> &skip, const PrimalityTestFunction &prime_test,
                              PrimalityTestArgs &&... args) {
    std::vector<BigInt> small_primes = read_numbers(Filenames::SmallPrimes);
    std::unordered_set<BigInt> small_primes_set(small_primes.begin(), small_primes.end());

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
