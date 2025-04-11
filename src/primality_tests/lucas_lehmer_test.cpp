#include "lucas_lehmer_test.h"

namespace BigPrimeLib {

PrimalityStatus lucas_lehmer_prime_test(const uint64_t &p) {
    BigInt m = (BigInt(1) << p) - 1;
    if (auto status = test_leq_3(m); status != PrimalityStatus::Uncertain) {
        return status;
    }
    BigInt s = 4;
    for (uint64_t k = 1; k + 1 < p; ++k) {
        s = (s * s - 2) % m;
    }
    if (s == 0) {
        return PrimalityStatus::Prime;
    } else {
        return PrimalityStatus::Composite;
    }
}

}
