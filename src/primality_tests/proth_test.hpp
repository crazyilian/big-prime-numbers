#pragma once

#include "common.h"
#include "random.hpp"
#include "primality_utils.h"

namespace BigPrimeLib {

inline PrimalityStatus proth_prime_test_base(const BigInt &n, const BigInt &base) {
    BigInt b = Math::powm(base, (n - 1) >> 1, n);
    if (b == n - 1) {
        return PrimalityStatus::Prime;
    } else if (b == 1) {
        return PrimalityStatus::Uncertain;
    } else {
        return PrimalityStatus::Composite;
    }
}

template<class Iterator>
PrimalityStatus proth_prime_test_iter(const BigInt &n, Iterator base_begin, Iterator base_end) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    assert(Math::msb(n - 1) < Math::lsb(n - 1) * 2);
    for (auto it = base_begin; it != base_end; ++it) {
        BigInt base = *it;
        auto status = proth_prime_test_base(n, base);
        if (status != PrimalityStatus::Uncertain) {
            return status;
        }
    }
    return PrimalityStatus::Uncertain;
}

template<class RandomT = Random<>>
PrimalityStatus proth_prime_test(const BigInt &n, size_t times, RandomT &rnd) {
    // n = k*2^s+1, k<2^s
    assert(n > 2);
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    RandomSequence rndseq([&rnd, &n]() { return rnd.uniform(2, n - 1); }, times);
    return proth_prime_test_iter(n, rndseq.begin(), rndseq.end());
}

template<class Iterator>
PrimalityStatus proth_prime_test_iter_assume_composite(const BigInt &n, Iterator base_begin, Iterator base_end) {
    return uncertain2composite(proth_prime_test_iter(n, base_begin, base_end));
}

template<class RandomT = Random<>>
PrimalityStatus proth_prime_test_assume_composite(const BigInt &n, size_t times, RandomT &rnd) {
    return uncertain2composite(proth_prime_test(n, times, rnd));
}

}
