#include "trial_test.h"

namespace BigPrimeLib {

TrialPrimeTester::TrialPrimeTester() {}

const PrimalityStatus &TrialPrimeTester::on_uncertain() const {
    return on_uncertain_;
}

PrimalityStatus TrialPrimeTester::test_raw(const BigInt &n) {
    assert(n % 2 == 1);
    for (BigInt d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Prime;
}

}
