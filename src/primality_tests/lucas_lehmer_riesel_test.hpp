#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

template<typename T>
void jacobi_iteration(uint64_t &a, const T &n, int &res) {
    while (a % 2 == 0) {
        a /= 2;
        if (n % 8 == 3 || n % 8 == 5) {
            res *= -1;
        }
    }
    if (n % 4 == 3 && a % 4 == 3) {
        res *= -1;
    }
}

inline int jacobi(uint64_t a, const BigInt &big_n) {
    assert(big_n > 0 && big_n % 2 == 1);
    a = static_cast<uint64_t>(a % big_n);
    if (a == 0) {
        return big_n == 1 ? 1 : 0;
    }
    int res = 1;
    jacobi_iteration(a, big_n, res);
    uint64_t n = static_cast<uint64_t>(big_n % a);
    std::swap(n, a);
    while (a != 0) {
        jacobi_iteration(a, n, res);
        n %= a;
        std::swap(a, n);
    }
    return n == 1 ? res : 0;
}

inline BigInt modinv2(const BigInt &mod) {
    BigInt inv;
    mpz_invert(inv.backend().data(), BigInt(2).backend().data(), mod.backend().data());
    return inv;
}

inline BigInt lucas_sequence_p1(const BigInt &k, const uint64_t &p, const BigInt &mod) {
    if (k == 1) {
        return p;
    }
    BigInt u = 1;
    BigInt v = p;
    BigInt inv2 = modinv2(mod);
    uint64_t i = Math::msb(k);
    uint64_t d = p * p - 4;
    do {
        --i;
        u = (u * v) % mod;
        v = (v * v - 2 + mod) % mod;
        if (Math::bit_test(k, i)) {
            BigInt u_new = ((p * u + v) % mod * inv2) % mod;
            BigInt v_new = ((d * u + p * v) % mod * inv2) % mod;
            u = std::move(u_new);
            v = std::move(v_new);
        }
    } while (i > 0);

    return v;
}

inline PrimalityStatus lucas_lehmer_riesel_prime_test(const BigInt &k, const uint64_t &n) {
    assert(n > 0);
    BigInt tmp = Math::pow(BigInt(2), n);
    assert(tmp > k);
    BigInt N = k * tmp - 1;
    if (auto status = test_leq_3(N); status != PrimalityStatus::Uncertain) {
        return status;
    }

    uint64_t p = 4;
    if (k % 3 == 0) {
        for (p = 3; jacobi(p - 2, N) != 1 || jacobi(p + 2, N) != -1; ++p) {}
    }

    auto v = lucas_sequence_p1(k, p, N);
    for (size_t i = 1; i + 1 < n; ++i) {
        v = (v * v - 2) % N;
    }
    if (v % N == 0) {
        return PrimalityStatus::Prime;
    } else {
        return PrimalityStatus::Composite;
    }
}

}
