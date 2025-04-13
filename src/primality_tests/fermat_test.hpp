#pragma once

#include "common.h"
#include "primality_utils.h"
#include "random.hpp"

namespace BigPrimeLib {

inline PrimalityStatus fermat_prime_test_base(const BigInt &n, const BigInt &base) {
    if (Math::powm(base, n - 1, n) == 1) {
        return PrimalityStatus::Uncertain;
    } else {
        return PrimalityStatus::Composite;
    }
}

template<class Iterator>
PrimalityStatus fermat_prime_test_iter(const BigInt &n, Iterator base_begin, Iterator base_end) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    for (auto it = base_begin; it != base_end; ++it) {
        BigInt base = *it;
        if (fermat_prime_test_base(n, base) == PrimalityStatus::Composite) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Uncertain;
}

template<class RandomT = Random<>>
PrimalityStatus fermat_prime_test(const BigInt &n, size_t times, RandomT &rnd) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    RandomSequence rndseq([&rnd, &n]() { return rnd.uniform(2, n - 1); }, times);
    return fermat_prime_test_iter(n, rndseq.begin(), rndseq.end());
}

template<class Iterator>
PrimalityStatus fermat_prime_test_iter_assume_prime(const BigInt &n, Iterator base_begin, Iterator base_end) {
    return uncertain2prime(fermat_prime_test_iter(n, base_begin, base_end));
}

template<class RandomT = Random<>>
PrimalityStatus fermat_prime_test_assume_prime(const BigInt &n, size_t times, RandomT &rnd) {
    return uncertain2prime(fermat_prime_test(n, times, rnd));
}

}
