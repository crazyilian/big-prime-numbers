#pragma once

#include "common.h"
#include "primality_status.h"

namespace BigPrimeLib {

PrimalityStatus miller_rabin_prime_test_base(const BigInt &n, size_t s, const BigInt &t, const BigInt &base) {
    BigInt x = boost::multiprecision::powm(base, t, n);
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

template<class RandomGenerator>
PrimalityStatus miller_rabin_prime_test(const BigInt &n, size_t times, RandomGenerator &rng) {
    if (n <= 1) {
        return PrimalityStatus::NotApplicable;
    }
    if (n <= 3) {
        return PrimalityStatus::Prime;
    }
    UniformBigIntDistribution dist(2, n - 2);
    size_t s = boost::multiprecision::lsb(n - 1);
    BigInt t = n >> s;
    for (size_t i = 0; i < times; ++i) {
        BigInt base = dist(rng);
        if (miller_rabin_prime_test_base(n, s, t, base) == PrimalityStatus::Composite) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Uncertain;
}

}
