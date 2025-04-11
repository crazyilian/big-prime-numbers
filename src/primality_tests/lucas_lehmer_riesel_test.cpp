#include "lucas_lehmer_riesel_test.h"
#include "lucas_sequence.h"
#include "jacobi.hpp"

namespace BigPrimeLib {

PrimalityStatus lucas_lehmer_riesel_prime_test(const BigInt &k, const uint64_t &n) {
    assert(n > 0);
    BigInt tmp = BigInt(1) << n;
    assert(tmp > k);
    BigInt N = k * tmp - 1;
    if (auto status = test_leq_3(N); status != PrimalityStatus::Uncertain) {
        return status;
    }

    uint64_t p = 4;
    if (k % 3 == 0) {
        for (p = 3; jacobi(p - 2, N) != 1 || jacobi(p + 2, N) != -1; ++p) {}
    }

    auto v = LucasSequence(k, p, 1, N).v;
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
