#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

PrimalityStatus lucas_lehmer_riesel_prime_test(const BigInt &k, const uint64_t &n);

}
