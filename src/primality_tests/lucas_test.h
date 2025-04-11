#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

// lucas_test

PrimalityStatus lucas_test(const BigInt &n, const BigInt &p, const BigInt &q, int jacob);

PrimalityStatus lucas_test(const BigInt &n, const BigInt &p, const BigInt &q);

// strong_lucas_test

PrimalityStatus strong_lucas_test(const BigInt &n, const BigInt &d, const size_t &s, const BigInt &p, const BigInt &q);

PrimalityStatus strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q, int jacob);

PrimalityStatus strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q);

// extra_strong_lucas_test

PrimalityStatus extra_strong_lucas_test(const BigInt &n, const BigInt &d, const size_t &s, const BigInt &p,
                                        const BigInt &q);

PrimalityStatus extra_strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q, int jacob);

PrimalityStatus extra_strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q);

}
