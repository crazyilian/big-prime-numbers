#pragma once

#include "common.h"

namespace BigPrimeLib {

class LucasSequence {
public:
    BigInt u, v;
    BigInt p, q, mod;
    BigInt k;

public:
    LucasSequence(const BigInt &p, const BigInt &q, const BigInt &mod);
    LucasSequence(const BigInt &k, const BigInt &p, const BigInt &q, const BigInt &mod);

    void add1();
    void mul2();

private:
    BigInt d_, inv2_, qpow_;
};

}
