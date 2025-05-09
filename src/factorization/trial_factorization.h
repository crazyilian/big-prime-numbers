#pragma once

#include "common.h"
#include "factorization_utils.hpp"

namespace BigPrimeLib {

class TrialFactorizer {
public:
    TrialFactorizer();
    explicit TrialFactorizer(const PrimeTester &prime_tester);
    std::optional<BigInt> find_factor(const BigInt &n);
    PrimalityStatus prime_test(const BigInt &n);

private:
    std::optional<PrimeTester> prime_tester_;
};

}
