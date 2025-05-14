#pragma once

#include <boost/dynamic_bitset.hpp>
#include "common.h"
#include "sieve_eratosthenes.h"
#include "factorization_utils.h"

namespace BigPrimeLib {

class DixonFactorizer {
public:
    explicit DixonFactorizer(size_t b_lim);
    DixonFactorizer(const PrimeTester &prime_tester, size_t b_lim);
    PrimalityStatus primality_test(const BigInt &n);
    std::optional<BigInt> find_factor(const BigInt &n);

private:
    struct Rels {
        BigInt x, y;
        std::vector<size_t> exponents;
    };

    std::optional<std::vector<size_t>> factor_over_base(BigInt x);
    std::vector<boost::dynamic_bitset<>> nullspace_mod2(std::vector<boost::dynamic_bitset<>> mat);

private:
    std::optional<PrimeTester> prime_tester_;
    SieveEratosthenes sieve_;
};

}
