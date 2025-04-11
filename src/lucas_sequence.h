#pragma once

#include "common.h"

namespace BigPrimeLib {

struct LucasPair {
    BigInt u, v;
};

LucasPair lucas_sequence_p1(const BigInt &k, BigInt p, const BigInt &mod);
LucasPair lucas_sequence_pq(const BigInt &k, BigInt p, BigInt q, const BigInt &mod);


}
