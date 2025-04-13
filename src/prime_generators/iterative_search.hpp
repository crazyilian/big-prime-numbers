#pragma once

#include "common.h"
#include "primality_tests/primality_utils.h"
#include "random.hpp"
#include "except.h"

namespace BigPrimeLib {

namespace Except {

    class NoPrimesInRange : public std::runtime_error {
    public:
        explicit NoPrimesInRange(const BigInt &a, const BigInt &b)
            : std::runtime_error(std::format("No primes found in range [{}, {}]", to_string(a), to_string(b))) {}

    };

}

template<class PrimalityTestFunction, class... PrimalityTestArgs>
BigInt search_next_prime_in_range(const BigInt &s, const BigInt &l, const BigInt &r,
                                  const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
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
    throw Except::NoPrimesInRange(l, r);
}

template<class PrimalityTestFunction, class... PrimalityTestArgs>
BigInt search_next_prime(const BigInt &l, const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
    auto r = Math::max(BigInt(2), 2 * l);
    return search_next_prime_in_range(l, l, r, prime_test, std::forward<PrimalityTestArgs>(args)...);
}

}
