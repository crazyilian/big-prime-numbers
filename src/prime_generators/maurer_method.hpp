#pragma once

#include "common.h"
#include "iterative_search.hpp"
#include "primality_tests/miller_rabin_test.hpp"

namespace BigPrimeLib {


template<class RandomT = Random<>>
BigInt generate_prime_maurer(size_t bit_size, RandomT &rnd, size_t bit_margin = 20, size_t iterative_bit_limit = 64) {
    assert(bit_size > 1 && bit_margin >= 1 && bit_margin < iterative_bit_limit && iterative_bit_limit <= 64);
    if (bit_size <= iterative_bit_limit) {
        return generate_prime_in_range(BigInt(1) << (bit_size - 1), (BigInt(1) << bit_size) - 1, rnd,
                                       miller_rabin_prime_test_iter_assume_prime<std::vector<BigInt>::const_iterator>,
                                       kMillerRabinDeterministicBasesU64.begin(),
                                       kMillerRabinDeterministicBasesU64.end());
    }
    size_t new_bit_size = 0;
    do {
        double alpha = std::pow(2, rnd.uniform_real(-1, 0));
        new_bit_size = static_cast<size_t>(alpha * static_cast<double>(bit_size));
    } while (new_bit_size + bit_margin >= bit_size);
    BigInt q = generate_prime_maurer(new_bit_size, rnd, bit_margin, iterative_bit_limit);

    auto lb = ((BigInt(1) << (bit_size - 2)) + q - 1) / q;
    auto ub = ((BigInt(1) << (bit_size - 1)) - 1) / q;

    while (true) {
        auto r = rnd.uniform(lb, ub);
        auto p = 2 * r * q + 1;
        auto a = rnd.uniform(2, p - 2);
        auto b = Math::powm(a, 2 * r, p);
        if (b != 1 && Math::powm(b, q, p) == 1) {
            return p;
        }
    }
}

}
