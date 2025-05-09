#pragma once

#include "common.h"
#include "iterative_search.hpp"
#include "primality_tests/miller_rabin_test.hpp"

namespace BigPrimeLib {

// certificate

class MaurerPrimeCertificate;

class MaurerPrime {
public:
    BigInt p;
    std::unique_ptr<MaurerPrimeCertificate> cert;

    explicit MaurerPrime(const BigInt &p);
    MaurerPrime(const BigInt &p, const BigInt &r, const BigInt &a, MaurerPrime &q);

    bool verify_assuming_prime_base() const;
    BigInt get_cert_base() const;
};


class MaurerPrimeCertificate {
public:
    BigInt r, a;
    MaurerPrime q;

    MaurerPrimeCertificate(const BigInt &r, const BigInt &a, MaurerPrime &q) : r(r), a(a), q(std::move(q)) {}
};

inline MaurerPrime::MaurerPrime(const BigInt &p) : p(p) {}

inline MaurerPrime::MaurerPrime(const BigInt &p, const BigInt &r, const BigInt &a, MaurerPrime &q)
    : p(p), cert(std::make_unique<MaurerPrimeCertificate>(r, a, q)) {}

inline bool MaurerPrime::verify_assuming_prime_base() const {
    if (cert == nullptr) {
        return true;
    }
    if (p != 2 * cert->r * cert->q.p + 1) {
        return false;
    }
    auto b = Math::powm(cert->a, 2 * cert->r, p);
    if (b == 1 || Math::powm(b, cert->q.p, p) != 1) {
        return false;
    }
    return cert->q.verify_assuming_prime_base();
}

inline BigInt MaurerPrime::get_cert_base() const {
    return cert == nullptr ? p : cert->q.get_cert_base();
}


// generation

namespace detail {

    template<class RandomGenerator>
    BigInt generate_prime_maurer_small_bitsize(size_t bit_size, Random<RandomGenerator> &rnd) {
        PrimeTester prime_tester = MillerRabinPrimeTesterIter(kMillerRabinDeterministicBasesU64.begin(),
                                                              kMillerRabinDeterministicBasesU64.end());
        auto p = generate_prime_in_range(BigInt(1) << (bit_size - 1), (BigInt(1) << bit_size) - 1, rnd, prime_tester);
        return p.value();
    }

    template<class RandomGenerator>
    size_t generate_prime_maurer_new_bitsize(size_t bit_size, size_t bit_margin, Random<RandomGenerator> &rnd) {
        size_t new_bit_size = 0;
        do {
            double alpha = std::pow(2, rnd.uniform_real(-1, 0));
            new_bit_size = static_cast<size_t>(alpha * static_cast<double>(bit_size));
        } while (new_bit_size + bit_margin >= bit_size);
        return new_bit_size;
    }

    template<class RandomGenerator>
    std::array<BigInt, 3> generate_prime_maurer_find_p_r_a(size_t bit_size, const BigInt &q,
                                                           Random<RandomGenerator> &rnd) {
        auto lb = ((BigInt(1) << (bit_size - 2)) + q - 1) / q;
        auto ub = ((BigInt(1) << (bit_size - 1)) - 1) / q;
        while (true) {
            auto r = rnd.uniform(lb, ub);
            auto p = 2 * r * q + 1;
            auto a = rnd.uniform(2, p - 2);
            auto b = Math::powm(a, 2 * r, p);
            if (b != 1 && Math::powm(b, q, p) == 1) {
                return {p, r, a};
            }
        }
    }

}

template<class RandomGenerator>
MaurerPrime generate_prime_maurer_with_cert(size_t bit_size, Random<RandomGenerator> &rnd,
                                            size_t bit_margin = 20, size_t iterative_bit_limit = 64) {
    assert(bit_size > 1 && bit_margin >= 1 && bit_margin < iterative_bit_limit && iterative_bit_limit <= 64);

    if (bit_size <= iterative_bit_limit) {
        return MaurerPrime(detail::generate_prime_maurer_small_bitsize(bit_size, rnd));
    }
    size_t new_bit_size = detail::generate_prime_maurer_new_bitsize(bit_size, bit_margin, rnd);
    MaurerPrime q = generate_prime_maurer_with_cert(new_bit_size, rnd, bit_margin, iterative_bit_limit);
    auto [p, r, a] = detail::generate_prime_maurer_find_p_r_a(bit_size, q.p, rnd);
    return MaurerPrime(p, r, a, q);
}

template<class RandomGenerator>
BigInt generate_prime_maurer(size_t bit_size, Random<RandomGenerator> &rnd,
                             size_t bit_margin = 20, size_t iterative_bit_limit = 64) {
    assert(bit_size > 1 && bit_margin >= 1 && bit_margin < iterative_bit_limit && iterative_bit_limit <= 64);

    if (bit_size <= iterative_bit_limit) {
        return detail::generate_prime_maurer_small_bitsize(bit_size, rnd);
    }
    size_t new_bit_size = detail::generate_prime_maurer_new_bitsize(bit_size, bit_margin, rnd);
    BigInt q = generate_prime_maurer(new_bit_size, rnd, bit_margin, iterative_bit_limit);
    auto [p, r, a] = detail::generate_prime_maurer_find_p_r_a(bit_size, q, rnd);
    return p;
}

}
