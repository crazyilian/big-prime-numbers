#pragma once

#include "common.h"
#include "primality_utils.h"
#include "jacobi.hpp"
#include "lucas_sequence.h"

namespace BigPrimeLib {

// lucas_test

template<typename T>
PrimalityStatus lucas_test(const BigInt &n, const T &p, const T &q, int jacob) {
    assert(n > 0 && jacob != 0 && jacob == jacobi(p * p - 4 * q, n));
    auto ls = LucasSequence(n - jacob, p, q, n);
    if (ls.u() % n == 0) {
        return PrimalityStatus::Uncertain;
    }
    return PrimalityStatus::Composite;
}

template<typename T>
PrimalityStatus lucas_test(const BigInt &n, const T &p, const T &q) {
    assert(n > 0 && n % 2 == 1 && Math::gcd(p * p - 4 * q, n) != 1);
    T D = p * p - 4 * q;
    return lucas_test(n, p, q, jacobi(D, n));
}

// strong_lucas_test

template<typename T>
PrimalityStatus strong_lucas_test(const BigInt &n, const BigInt &d, const size_t &s, const T &p, const T &q) {
    assert(n > 0 && s > 0 && Math::gcd(n, q) == 1 && n - (d << s) == jacobi(p * p - 4 * q, n));
    if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    }
    auto ls = LucasSequence(d, p, q, n);
    if (ls.u() % n == 0 || ls.v() % n == 0) {
        return PrimalityStatus::Uncertain;
    }
    for (size_t r = 1; r < s; ++r) {
        ls.mul2();
        if (ls.v() % n == 0) {
            return PrimalityStatus::Uncertain;
        }
    }
    return PrimalityStatus::Composite;
}

template<typename T>
PrimalityStatus strong_lucas_test(const BigInt &n, const T &p, const T &q, int jacob) {
    assert(n > 0 && jacob != 0 && jacob == jacobi(p * p - 4 * q, n));
    BigInt d = n - jacob;
    size_t s = Math::lsb(d);
    d >>= s;
    return strong_lucas_test(n, d, s, p, q);
}

template<typename T>
PrimalityStatus strong_lucas_test(const BigInt &n, const T &p, const T &q) {
    assert(n > 0 && n % 2 == 1 && Math::gcd(p * p - 4 * q, n) != 1);
    T D = p * p - 4 * q;
    return strong_lucas_test(n, p, q, jacobi(D, n));
}

// stronger_lucas_test

template<typename T>
PrimalityStatus stronger_lucas_test(const BigInt &n, const BigInt &d, const size_t &s, const T &p, const T &q,
                                    int jacob) {
    assert(n > 0 && s > 0 && Math::gcd(n, q) == 1 && jacob != 0 && jacob == jacobi(p * p - 4 * q, n)
        && n - jacob == d << s);
    if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    }

    // strong Lucas test
    bool composite = true;
    auto ls = LucasSequence(d, p, q, n);
    if (ls.u() % n == 0 || ls.v() % n == 0) {
        composite = false;
    }
    for (size_t r = 1; r < s; ++r) {
        ls.mul2();
        if (composite && ls.v() % n == 0) {
            composite = false;
        }
    }
    if (composite) {
        return PrimalityStatus::Composite;
    }

    // Euler–Jacobi test
    int jacobqn = jacobi(q, n);
    if (jacobqn == 0) {
        return PrimalityStatus::Composite;
    }
    if (q != 1 && q != -1 && (ls.qpow() - q * jacobqn + n) % n != 0) {
        return PrimalityStatus::Composite;
    }

    ls.mul2();
    if (jacob == -1) {
        // V_{n+1} = 2Q (mod n)
        if ((2 * q - ls.v() + n) % n != 0) {
            return PrimalityStatus::Composite;
        }
    } else {
        // V_{n-1} = 2 (mod n)
        if ((ls.v() - 2 + n) % n) {
            return PrimalityStatus::Composite;
        }
    }

    return PrimalityStatus::Uncertain;
}

template<typename T>
PrimalityStatus stronger_lucas_test(const BigInt &n, const T &p, const T &q, int jacob) {
    assert(n > 0 && Math::gcd(n, q) == 1 && jacob != 0 && jacob == jacobi(p * p - 4 * q, n));
    BigInt d = n - jacob;
    size_t s = Math::lsb(d);
    d >>= s;
    return stronger_lucas_test(n, d, s, p, q, jacob);
}

template<typename T>
PrimalityStatus stronger_lucas_test(const BigInt &n, const T &p, const T &q) {
    assert(n > 0 && n % 2 == 1 && Math::gcd(p * p - 4 * q, n) != 1);
    T D = p * p - 4 * q;
    return stronger_lucas_test(n, p, q, jacobi(D, n));
}


}
