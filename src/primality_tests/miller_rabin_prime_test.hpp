#pragma once

#include "common.h"
#include "random.hpp"
#include "primality_utils.h"

namespace BigPrimeLib {

inline PrimalityStatus miller_rabin_prime_test_base(const BigInt &n, size_t s, const BigInt &t, const BigInt &base) {
    BigInt x = Math::powm(base, t, n);
    if (x == 1 || x == n - 1) {
        return PrimalityStatus::Uncertain;
    }
    for (size_t i = 0; i + 1 < s; ++i) {
        x = x * x % n;
        if (x == 1) {
            return PrimalityStatus::Composite;
        }
        if (x == n - 1) {
            return PrimalityStatus::Uncertain;
        }
    }
    return PrimalityStatus::Composite;
}

template<class Iterator>
PrimalityStatus miller_rabin_prime_test_iter(const BigInt &n, Iterator base_begin, Iterator base_end) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    size_t s = Math::lsb(n - 1);
    BigInt t = n >> s;
    for (auto it = base_begin; it != base_end; ++it) {
        BigInt base = *it;
        if (miller_rabin_prime_test_base(n, s, t, base) == PrimalityStatus::Composite) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Uncertain;
}

template<class RandomGenerator = DefaultRandomGenerator>
PrimalityStatus miller_rabin_prime_test(const BigInt &n, size_t times, Random<RandomGenerator> &rnd) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    RandomSequence rndseq([&rnd, &n]() { return rnd.uniform(2, n - 2); }, times);
    return miller_rabin_prime_test_iter(n, rndseq.begin(), rndseq.end());
}

}
