#pragma once

#include "common.h"
#include "primality_tests/primality_utils.hpp"
#include "random.hpp"

namespace BigPrimeLib {

inline std::optional<BigInt> search_next_prime_in_range(const BigInt &s, const BigInt &l, const BigInt &r,
                                                        PrimeTester &prime_tester) {
    assert(l <= s && s <= r);
    if (s <= 2 && 2 <= r) {
        return 2;
    }
    for (BigInt x = s + (1 - s % 2); x <= r; x += 2) {
        if (prime_tester.test(x) == PrimalityStatus::Prime) {
            return x;
        }
    }
    if (l <= 2 && 2 < s) {
        return 2;
    }
    for (BigInt x = l + (1 - l % 2); x < s; x += 2) {
        if (prime_tester.test(x) == PrimalityStatus::Prime) {
            return x;
        }
    }
    return std::nullopt;
}

inline BigInt search_next_prime(const BigInt &l, PrimeTester &prime_tester) {
    auto r = Math::max(BigInt(2), 2 * l);
    auto p = search_next_prime_in_range(l, l, r, prime_tester);
    return p.value();
}


template<class RandomGenerator>
std::optional<BigInt> generate_prime_in_range(const BigInt &l, const BigInt &r, Random<RandomGenerator> &rnd,
                                              PrimeTester &prime_tester) {
    BigInt s = rnd.uniform(l, r);
    return search_next_prime_in_range(s, l, r, prime_tester);
}

}
