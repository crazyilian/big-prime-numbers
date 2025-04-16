#pragma once

#include "common.h"
#include "primality_tests/primality_utils.hpp"

namespace BigPrimeLib {

class Factorizer {
public:
    std::unique_ptr<PrimeTester> prime_tester;

public:
    Factorizer();
    Factorizer(const Factorizer &f);
    explicit Factorizer(const PrimeTester &prime_tester);
    virtual ~Factorizer() = default;
    virtual std::unique_ptr<Factorizer> clone() const = 0;

    virtual std::optional<BigInt> find_factor(const BigInt &n) = 0;

    std::vector<BigInt> factorization(const BigInt &n);

public:
    void factorization_accumulate(const BigInt &n, std::vector<BigInt> &result);
};

}
