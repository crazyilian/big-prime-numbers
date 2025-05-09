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

LucasSequence::LucasSequence(const BigInt &p, const BigInt &q, const BigInt &mod)
    : p_(modpos(p, mod)), q_(modpos(q, mod)), mod_(mod) {
    assert(mod % 2 == 1);
    inv2_ = modinv2(mod);
    d_ = modpos(p * p - 4 * q, mod);
    qpow_ = 1;
    k_ = 0;
    u_ = 0;
    v_ = 2;
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

void LucasSequence::add1() {
    BigInt u_new = ((p_ * u_ + v_) % mod_ * inv2_) % mod_;
    BigInt v_new = ((d_ * u_ + p_ * v_) % mod_ * inv2_) % mod_;
    u_ = std::move(u_new);
    v_ = std::move(v_new);
    qpow_ = qpow_ * q_ % mod_;
    k_++;
}

void LucasSequence::mul2() {
    u_ = u_ * v_ % mod_;
    v_ = modpos(v_ * v_ - (2 * qpow_), mod_);
    qpow_ = qpow_ * qpow_ % mod_;
    k_ *= 2;
}

const BigInt &LucasSequence::u() const { return u_; }
const BigInt &LucasSequence::v() const { return v_; }
const BigInt &LucasSequence::qpow() const { return qpow_; }
const BigInt &LucasSequence::k() const { return k_; }

}
