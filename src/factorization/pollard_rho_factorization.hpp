#pragma once

#include "common.h"
#include "factorization_utils.hpp"
#include "random.hpp"

namespace BigPrimeLib {

template<class RandomGenerator>
class PollardRhoFactorizer {
public:
    PollardRhoFactorizer(const Random<RandomGenerator> &rnd, std::optional<size_t> base_times,
                         std::optional<size_t> iter_times)
        : rnd_(rnd), base_times_(base_times), iter_times_(iter_times) {}

    PollardRhoFactorizer(const PrimeTester &prime_tester, const Random<RandomGenerator> &rnd,
                         std::optional<size_t> base_times, std::optional<size_t> iter_times)
        : prime_tester_(prime_tester), rnd_(rnd), base_times_(base_times), iter_times_(iter_times) {}

    PrimalityStatus prime_test(const BigInt &n) {
        return prime_tester_ ? prime_tester_->test(n) : PrimalityStatus::Uncertain;
    }

    std::optional<BigInt> find_factor(const BigInt &n) {
        if (n % 2 == 0) {
            return 2;
        }
        for (size_t base_i = 0; !base_times_.has_value() || base_i < base_times_.value(); ++base_i) {
            BigInt x = rnd_.uniform(0, n - 1);
            BigInt y = x;
            BigInt c;
            do {
                c = rnd_.uniform(1, n - 1);
            } while (c == n - 2);
            auto f = [&n, &c](const BigInt &x) { return (x * x + c) % n; };
            for (size_t iter_i = 0; !iter_times_.has_value() || iter_i < iter_times_.value(); ++iter_i) {
                x = f(x);
                y = f(f(y));
                BigInt d = Math::gcd(Math::abs(x - y), n);
                if (d == n) {
                    break;
                } else if (d != 1) {
                    return d;
                }
            }
        }
        return std::nullopt;
    }

private:
    std::optional<PrimeTester> prime_tester_;
    Random<RandomGenerator> rnd_;
    std::optional<size_t> base_times_, iter_times_;
};

}
