#pragma once

#include <boost/dynamic_bitset.hpp>
#include "common.h"
#include "factorization_utils.h"
#include "sieve_eratosthenes.h"

namespace BigPrimeLib {


class DixonFactorizer : public Factorizer {
public:
    SieveEratosthenes sieve;

public:
    explicit DixonFactorizer(size_t b_lim);
    DixonFactorizer(const PrimeTester &prime_tester, size_t b_lim);

    std::unique_ptr<Factorizer> clone() const override;

private:
    struct Rels {
        BigInt x, y;
        std::vector<size_t> exponents;
    };

    std::optional<std::vector<size_t>> factor_over_base(BigInt x);
    std::vector<boost::dynamic_bitset<>> nullspace_mod2(std::vector<boost::dynamic_bitset<>> mat);

public:
    std::optional<BigInt> find_factor(const BigInt &n) override;
};

}
