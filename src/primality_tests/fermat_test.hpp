#pragma once

#include "common.hpp"
#include "primality_status.h"

namespace BigPrimeLib {

inline PrimalityStatus fermat_prime_test_base(const BigInt &n, const BigInt &base) {
    if (boost::multiprecision::powm(base, n - 1, n) == 1) {
        return PrimalityStatus::Uncertain;
    } else {
        return PrimalityStatus::Composite;
    }
}

template<class RandomGenerator = DefaultRandomGenerator>
PrimalityStatus fermat_prime_test(const BigInt &n, size_t times, Random<RandomGenerator> &rnd) {
    if (n <= 1) {
        return PrimalityStatus::NotApplicable;
    }
    if (n == 2) {
        return PrimalityStatus::Prime;
    }
    for (size_t i = 0; i < times; ++i) {
        BigInt base = rnd.uniform(2, n - 1);
        if (fermat_prime_test_base(n, base) == PrimalityStatus::Composite) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Uncertain;
}

}
