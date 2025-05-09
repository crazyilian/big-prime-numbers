#pragma once

#include "common.h"

namespace BigPrimeLib {

class LucasSequence {
public:
    LucasSequence(const BigInt &p, const BigInt &q, const BigInt &mod);
    LucasSequence(const BigInt &k, const BigInt &p, const BigInt &q, const BigInt &mod);

    void add1();
    void mul2();

    const BigInt &u() const;
    const BigInt &v() const;
    const BigInt &qpow() const;
    const BigInt &k() const;

private:
    BigInt d_, inv2_;

    BigInt u_, v_;
    BigInt p_, q_, mod_;
    BigInt k_, qpow_;
};

}
