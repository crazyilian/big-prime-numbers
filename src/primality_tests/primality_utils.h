#pragma once

#include "common.h"

namespace BigPrimeLib {

enum class PrimalityStatus {
    Prime,
    Composite,
    Uncertain,
    NotApplicable
};

const char *to_string(PrimalityStatus s);

PrimalityStatus test_leq_3(const BigInt &n);

PrimalityStatus uncertain2prime(PrimalityStatus s);

}
