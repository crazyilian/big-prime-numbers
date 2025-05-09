#include "trial_factorization.h"
#include "factorization_utils.hpp"

namespace BigPrimeLib {

TrialFactorizer::TrialFactorizer() {}

TrialFactorizer::TrialFactorizer(const PrimeTester &prime_tester) : prime_tester_(prime_tester) {}

PrimalityStatus TrialFactorizer::primality_test(const BigInt &n) {
    return prime_tester_ ? prime_tester_->test(n) : PrimalityStatus::Uncertain;
}

std::optional<BigInt> TrialFactorizer::find_factor(const BigInt &n) {
    if (n % 2 == 0) {
        return 2;
    }
    for (BigInt d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return d;
        }
    }
    return std::nullopt;
}

}
