#pragma once

#include "common.h"
#include "factorization_utils.h"

namespace BigPrimeLib {

class FermatFactorizer {
public:
    FermatFactorizer();
    explicit FermatFactorizer(const PrimeTester &prime_tester);
    PrimalityStatus primality_test(const BigInt &n);
    std::optional<BigInt> find_factor(const BigInt &n);

private:
    std::optional<PrimeTester> prime_tester_;
};

}
