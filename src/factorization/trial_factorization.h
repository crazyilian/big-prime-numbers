#pragma once

#include "common.h"

namespace BigPrimeLib {

std::optional<BigInt> trial_find_factor(const BigInt &n);

std::vector<BigInt> trial_factorization(const BigInt &n);

}
