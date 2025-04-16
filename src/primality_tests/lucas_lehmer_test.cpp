#include "lucas_lehmer_test.h"

namespace BigPrimeLib {

LucasLehmerPrimeTester::LucasLehmerPrimeTester() : PrimeTester(PrimalityStatus::Uncertain) {}

PrimalityStatus LucasLehmerPrimeTester::test_raw(const BigInt &n) {
    // n = 2^p-1, p is prime
    assert(n > 0);
    uint64_t p = Math::lsb(n + 1);
    assert(p > 0);
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    BigInt s = 4;
    for (uint64_t k = 1; k + 1 < p; ++k) {
        s = (s * s - 2) % n;
    }
    if (s == 0) {
        return PrimalityStatus::Prime;
    } else {
        return PrimalityStatus::Composite;
    }
}

std::unique_ptr<PrimeTester> LucasLehmerPrimeTester::clone() const {
    return std::make_unique<LucasLehmerPrimeTester>(*this);
}

}
