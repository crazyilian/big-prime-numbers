#pragma once

#include "common.h"
#include "factorization_utils.h"

namespace BigPrimeLib {

class TrialFactorizer : public Factorizer {
public:
    TrialFactorizer();
    explicit TrialFactorizer(const PrimeTester &prime_tester);
    std::optional<BigInt> find_factor(const BigInt &n) override;
    std::unique_ptr<Factorizer> clone() const override;
};

std::optional<BigInt> trial_find_factor(const BigInt &n);

std::vector<BigInt> trial_factorization(const BigInt &n);

}
