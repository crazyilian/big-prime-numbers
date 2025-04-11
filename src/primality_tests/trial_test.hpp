#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

inline PrimalityStatus trial_prime_test(const BigInt &n) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    }
    for (BigInt d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Prime;
}

}
