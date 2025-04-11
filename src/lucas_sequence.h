#pragma once

#include "common.h"

namespace BigPrimeLib {

class LucasState {
public:
    BigInt u, v;
    BigInt p, q, mod;
    BigInt k;

public:
    LucasState(const BigInt &p, const BigInt &q, const BigInt &mod);
    LucasState(const BigInt &k, const BigInt &p, const BigInt &q, const BigInt &mod);

    void add1();
    void mul2();

private:
    BigInt d_, inv2_, qpow_;
};

}
