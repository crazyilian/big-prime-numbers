#pragma once

#include "common.h"
#include "factorization_utils.h"

namespace BigPrimeLib {

class FermatFactorizer : public Factorizer {
public:
    FermatFactorizer();
    explicit FermatFactorizer(const PrimeTester &prime_tester);
    std::optional<BigInt> find_factor(const BigInt &n) override;
    std::unique_ptr<Factorizer> clone() const override;
};

}
