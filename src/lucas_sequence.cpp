#include "common.h"
#include "lucas_sequence.h"

namespace BigPrimeLib {

namespace {

    BigInt modinv2(const BigInt &mod) {
        BigInt inv;
        mpz_invert(inv.backend().data(), BigInt(2).backend().data(), mod.backend().data());
        return inv;
    }

    BigInt modpos(const BigInt &x, const BigInt &mod) {
        BigInt y = x % mod;
        if (y < 0) {
            return y + mod;
        }
        return y;
    }

}

LucasSequence::LucasSequence(const BigInt &p, const BigInt &q,
                             const BigInt &mod) : p(modpos(p, mod)), q(modpos(q, mod)), mod(mod) {
    assert(mod % 2 == 1);
    inv2_ = modinv2(mod);
    d_ = modpos(p * p - 4 * q, mod);
    qpow_ = 1;
    k = 0;
    u = 0;
    v = 2;
}

void LucasSequence::add1() {
    BigInt u_new = ((p * u + v) % mod * inv2_) % mod;
    BigInt v_new = ((d_ * u + p * v) % mod * inv2_) % mod;
    u = std::move(u_new);
    v = std::move(v_new);
    qpow_ = qpow_ * q % mod;
    k++;
}

void LucasSequence::mul2() {
    u = u * v % mod;
    v = modpos(v * v - (2 * qpow_), mod);
    qpow_ = qpow_ * qpow_ % mod;
    k *= 2;
}

LucasSequence::LucasSequence(const BigInt &k, const BigInt &p, const BigInt &q,
                             const BigInt &mod) : LucasSequence(p, q, mod) {
    if (k == 0) {
        return;
    }
    add1();
    if (k == 1) {
        return;
    }
    uint64_t i = Math::msb(k);
    do {
        --i;
        mul2();
        if (Math::bit_test(k, i)) {
            add1();
        }
    } while (i > 0);
}

}
