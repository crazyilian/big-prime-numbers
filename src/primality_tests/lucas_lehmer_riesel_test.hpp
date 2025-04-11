#pragma once

#include "common.h"
#include "primality_utils.h"
#include "lucas_sequence.h"
#include "jacobi.hpp"

namespace BigPrimeLib {

inline PrimalityStatus lucas_lehmer_riesel_prime_test(const BigInt &k, const uint64_t &n) {
    assert(n > 0);
    BigInt tmp = Math::pow(BigInt(2), n);
    assert(tmp > k);
    BigInt N = k * tmp - 1;
    if (auto status = test_leq_3(N); status != PrimalityStatus::Uncertain) {
        return status;
    }

    uint64_t p = 4;
    if (k % 3 == 0) {
        for (p = 3; jacobi(p - 2, N) != 1 || jacobi(p + 2, N) != -1; ++p) {}
    }

    auto v = lucas_sequence_p1(k, p, N).v;
    for (size_t i = 1; i + 1 < n; ++i) {
        v = (v * v - 2) % N;
    }
    if (v % N == 0) {
        return PrimalityStatus::Prime;
    } else {
        return PrimalityStatus::Composite;
    }
}

}
