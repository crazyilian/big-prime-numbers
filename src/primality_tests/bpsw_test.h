#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

inline PrimalityStatus bpsw_fermat_prime_test(const BigInt &n, bool known_wieferich = true);
inline PrimalityStatus bpsw_prime_test(const BigInt &n, bool known_wieferich = true);

}
