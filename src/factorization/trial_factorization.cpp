#include "trial_factorization.h"
#include "factorization_utils.h"

namespace BigPrimeLib {

TrialFactorizer::TrialFactorizer() {}

TrialFactorizer::TrialFactorizer(const PrimeTester &prime_tester) : Factorizer(prime_tester) {}

std::unique_ptr<Factorizer> TrialFactorizer::clone() const {
    return std::make_unique<TrialFactorizer>(*this);
}

std::optional<BigInt> TrialFactorizer::find_factor(const BigInt &n) {
    assert(n > 0);
    if (n <= 3) {
        return std::nullopt;
    }
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
