#pragma once

#include "common.h"
#include "primality_tests/primality_utils.h"
#include "random.hpp"

namespace BigPrimeLib {

template<class PrimalityTestFunction, class... PrimalityTestArgs>
std::optional<BigInt> search_next_prime_in_range(const BigInt &s, const BigInt &l, const BigInt &r,
                                                 const PrimalityTestFunction &prime_test,
                                                 PrimalityTestArgs &&... args) {
    assert(l <= s && s <= r);
    if (s <= 2 && 2 <= r) {
        return 2;
    }
    for (BigInt x = s + (1 - s % 2); x <= r; x += 2) {
        if (prime_test(x, std::forward<PrimalityTestArgs>(args)...) == PrimalityStatus::Prime) {
            return x;
        }
    }
    if (l <= 2 && 2 < s) {
        return 2;
    }
    for (BigInt x = l + (1 - l % 2); x < s; x += 2) {
        if (prime_test(x, std::forward<PrimalityTestArgs>(args)...) == PrimalityStatus::Prime) {
            return x;
        }
    }
    return std::nullopt;
}

template<class PrimalityTestFunction, class... PrimalityTestArgs>
BigInt search_next_prime(const BigInt &l, const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
    auto r = Math::max(BigInt(2), 2 * l);
    auto p = search_next_prime_in_range(l, l, r, prime_test, std::forward<PrimalityTestArgs>(args)...);
    return p.value();
}


template<class RandomT = Random<>, class PrimalityTestFunction, class... PrimalityTestArgs>
std::optional<BigInt> generate_prime_in_range(const BigInt &l, const BigInt &r, RandomT &rnd,
                               const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
    auto s = rnd.uniform(l, r);
    return search_next_prime_in_range(s, l, r, prime_test, std::forward<PrimalityTestArgs>(args)...);
}

}
