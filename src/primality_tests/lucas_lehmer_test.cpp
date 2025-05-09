#include "lucas_lehmer_test.h"

namespace BigPrimeLib {

LucasLehmerPrimeTester::LucasLehmerPrimeTester() {}

const PrimalityStatus &LucasLehmerPrimeTester::on_uncertain() const {
    return on_uncertain_;
}

PrimalityStatus LucasLehmerPrimeTester::test_raw(const BigInt &n) {
    // n = 2^p-1, p is prime
    uint64_t p = Math::lsb(n + 1);
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

}
