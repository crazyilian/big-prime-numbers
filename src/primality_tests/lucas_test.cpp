#include "lucas_test.h"
#include "jacobi.hpp"
#include "lucas_sequence.h"

namespace BigPrimeLib {

// lucas_test

PrimalityStatus lucas_test(const BigInt &n, const BigInt &p, const BigInt &q, int jacob) {
    assert(n > 0);
    auto ls = LucasState(n - jacob, p, q, n);
    if (ls.u % n == 0) {
        return PrimalityStatus::Uncertain;
    }
    return PrimalityStatus::Composite;
}

PrimalityStatus lucas_test(const BigInt &n, const BigInt &p, const BigInt &q) {
    return lucas_test(n, p, q, jacobi(BigInt(p * p - 4 * q), n));
}

// strong_lucas_test

PrimalityStatus strong_lucas_test(const BigInt &n, const BigInt &d, const size_t &s, const BigInt &p, const BigInt &q) {
    assert(n > 0);
    auto ls = LucasState(d, p, q, n);
    if (ls.u % n == 0 || (s != 0 && ls.v % n == 0)) {
        return PrimalityStatus::Uncertain;
    }
    for (size_t r = 1; r < s; ++r) {
        ls.mul2();
        if (ls.v % n == 0) {
            return PrimalityStatus::Uncertain;
        }
    }
    return PrimalityStatus::Composite;
}

PrimalityStatus strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q, int jacob) {
    BigInt d = n - jacob;
    size_t s = Math::lsb(d);
    d >>= s;
    return strong_lucas_test(n, d, s, p, q);
}

PrimalityStatus strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q) {
    return strong_lucas_test(n, p, q, jacobi(BigInt(p * p - 4 * q), n));
}

// extra_strong_lucas_test

PrimalityStatus extra_strong_lucas_test(const BigInt &n, const BigInt &d, const size_t &s, const BigInt &p,
                                        const BigInt &q) {
    assert(n > 0);
    auto ls = LucasState(d, p, q, n);
    if ((ls.u % n == 0 && (ls.v % n == 2 || (ls.v + 2) % n == 0)) || (s != 0 && ls.v % n == 0)) {
        return PrimalityStatus::Uncertain;
    }
    for (size_t r = 1; r < s; ++r) {
        ls.mul2();
        if (ls.v % n == 0) {
            return PrimalityStatus::Uncertain;
        }
    }
    return PrimalityStatus::Composite;
}

PrimalityStatus extra_strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q, int jacob) {
    BigInt d = n - jacob;
    size_t s = Math::lsb(d);
    d >>= s;
    return extra_strong_lucas_test(n, d, s, p, q);
}

PrimalityStatus extra_strong_lucas_test(const BigInt &n, const BigInt &p, const BigInt &q) {
    return extra_strong_lucas_test(n, p, q, jacobi(BigInt(p * p - 4 * q), n));
}


}
