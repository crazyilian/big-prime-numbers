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

LucasPair lucas_sequence_p1(const BigInt &k, BigInt p, const BigInt &mod) {
    assert(mod % 2 == 1);
    if (k == 0) {
        return {0, 2};
    }
    p = modpos(p, mod);
    if (k == 1) {
        return {1, p};
    }
    BigInt u = 1;
    BigInt v = p;
    BigInt inv2 = modinv2(mod);
    BigInt d = p * p - 4;
    uint64_t i = Math::msb(k);
    do {
        --i;
        u = u * v % mod;
        v = modpos(v * v - 2, mod);
        if (Math::bit_test(k, i)) {
            BigInt u_new = ((p * u + v) % mod * inv2) % mod;
            BigInt v_new = ((d * u + p * v) % mod * inv2) % mod;
            u = std::move(u_new);
            v = std::move(v_new);
        }
    } while (i > 0);
    return {u, v};
}

LucasPair lucas_sequence_pq(const BigInt &k, BigInt p, BigInt q, const BigInt &mod) {
    assert(mod % 2 == 1);
    if (k == 0) {
        return {0, 2};
    }
    p = modpos(p, mod);
    q = modpos(q, mod);
    if (k == 1) {
        return {1, p};
    }
    BigInt u = 1;
    BigInt v = p;
    BigInt qpow = q;
    BigInt inv2 = modinv2(mod);
    BigInt d = p * p - 4;
    uint64_t i = Math::msb(k);
    do {
        --i;
        u = u * v % mod;
        v = modpos(v * v - (2 * qpow), mod);
        qpow = qpow * qpow % mod;
        if (Math::bit_test(k, i)) {
            BigInt u_new = ((p * u + v) % mod * inv2) % mod;
            BigInt v_new = ((d * u + p * v) % mod * inv2) % mod;
            u = std::move(u_new);
            v = std::move(v_new);
            qpow = qpow * q % mod;
        }
    } while (i > 0);
    return {u, v};
}

}
