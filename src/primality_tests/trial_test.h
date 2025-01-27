#pragma once

#include "common.hpp"
#include "primality_status.h"

namespace BigPrimeLib {

PrimalityStatus trial_prime_test(const BigInt &number);

}
