#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

inline PrimalityStatus bpsw_fermat_prime_test(const BigInt &n, bool known_wieferich = true,
                                              bool stronger_lucas = false);

inline PrimalityStatus bpsw_miller_prime_test(const BigInt &n, bool known_wieferich = true,
                                              bool stronger_lucas = false);

inline PrimalityStatus bpsw_fermat_prime_test_assume_prime(const BigInt &n, bool known_wieferich = true,
                                                           bool stronger_lucas = false);

inline PrimalityStatus bpsw_miller_prime_test_assume_prime(const BigInt &n, bool known_wieferich = true,
                                                           bool stronger_lucas = false);

}
