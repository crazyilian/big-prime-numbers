#pragma once

namespace BigPrimeLib {

namespace detail {

    template<typename T1, typename T2>
    void run_jacobi_iteration(T1 &a, const T2 &n, int &res) {
        while (a % 2 == 0) {
            a /= 2;
            if (n % 8 == 3 || n % 8 == 5) {
                res = -res;
            }
        }
        if (n % 4 == 3 && a % 4 == 3) {
            res = -res;
        }
    }

}

template<typename T>
int jacobi(T a, T n) {
    assert(n > 0 && n % 2 == 1);
    a %= n;
    if (a < 0) {
        a += n;
    }
    int res = 1;
    while (a != 0) {
        detail::run_jacobi_iteration(a, n, res);
        n %= a;
        std::swap(a, n);
    }
    return n == 1 ? res : 0;
}

inline int jacobi(const BigInt &a, const BigInt &n) {
    assert(n > 0 && n % 2 == 1);
    return mpz_jacobi(a.backend().data(), n.backend().data());
}

template<typename T1, typename T2>
int jacobi(T1 a, const T2 &big_n) {
    assert(big_n > 0 && big_n % 2 == 1);
    if (a == 0) {
        return big_n == 1 ? 1 : 0;
    }
    int res = 1;
    if (a < 0) {
        if (big_n % 4 == 3) {
            res = -res;
        }
        a = -a;
    }
    T1 n;
    if (big_n > a) {
        detail::run_jacobi_iteration(a, big_n, res);
        n = big_n % a;
        std::swap(n, a);
    } else {
        n = big_n;
        a %= n;
    }
    return res * jacobi(a, n);
}

}
