#include "lucas_lehmer_riesel_test.h"
#include "lucas_sequence.h"
#include "jacobi.hpp"

namespace BigPrimeLib {

LucasLehmerRieselPrimeTester::LucasLehmerRieselPrimeTester() {}

const PrimalityStatus &LucasLehmerRieselPrimeTester::on_uncertain() const {
    return on_uncertain_;
}

PrimalityStatus LucasLehmerRieselPrimeTester::test_raw(const BigInt &n) {
    // n = k * 2^s - 1, k<2^s
    assert(n > 0 && n % 2 == 1);
    uint64_t s = Math::lsb(n + 1);
    assert(Math::msb(n + 1) < s * 2);
    BigInt k = (n + 1) >> s;

    uint64_t p = 4;
    if (k % 3 == 0) {
        for (p = 3; jacobi(p - 2, n) != 1 || jacobi(p + 2, n) != -1; ++p) {}
    }

    auto v = LucasSequence(k, p, 1, n).v();
    for (size_t i = 1; i + 1 < s; ++i) {
        v = (v * v - 2) % n;
    }
    if (v % n == 0) {
        return PrimalityStatus::Prime;
    } else {
        return PrimalityStatus::Composite;
    }
}

}
