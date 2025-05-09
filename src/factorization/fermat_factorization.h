#pragma once

#include "common.h"
#include "factorization_utils.hpp"

namespace BigPrimeLib {

class FermatFactorizer {
public:
    FermatFactorizer();
    explicit FermatFactorizer(const PrimeTester &prime_tester);
    std::optional<BigInt> find_factor(const BigInt &n);
    PrimalityStatus primality_test(const BigInt &n);

private:
    std::optional<PrimeTester> prime_tester_;
};

}
