#include "trial_test.h"

namespace BigPrimeLib {

TrialPrimeTester::TrialPrimeTester() : PrimeTester(PrimalityStatus::Uncertain) {}

PrimalityStatus TrialPrimeTester::test_raw(const BigInt &n) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    }
    if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    }
    for (BigInt d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Prime;
}

std::unique_ptr<PrimeTester> TrialPrimeTester::clone() const {
    return std::make_unique<TrialPrimeTester>(*this);
}

}
