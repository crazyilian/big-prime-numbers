#pragma once

#include "common.h"
#include "factorization_utils.h"
#include "random.hpp"

namespace BigPrimeLib {

template<class RandomGenerator>
class RhoPollardFactorizer : public Factorizer {
public:
    Random<RandomGenerator> rnd;
    std::optional<size_t> base_times, iter_times;

public:
    RhoPollardFactorizer(Random<RandomGenerator> rnd, std::optional<size_t> base_times,
                         std::optional<size_t> iter_times)
        : rnd(rnd), base_times(base_times), iter_times(iter_times) {}

    RhoPollardFactorizer(const PrimeTester &prime_tester, Random<RandomGenerator> rnd,
                         std::optional<size_t> base_times, std::optional<size_t> iter_times)
        : Factorizer(prime_tester), rnd(rnd), base_times(base_times), iter_times(iter_times) {}

    std::unique_ptr<Factorizer> clone() const override {
        return std::make_unique<RhoPollardFactorizer>(*this);
    }

    std::optional<BigInt> find_factor(const BigInt &n) override {
        assert(n > 0);
        if (n <= 3) {
            return std::nullopt;
        }
        if (n % 2 == 0) {
            return 2;
        }
        for (size_t base_i = 0; !base_times.has_value() || base_i < base_times.value(); ++base_i) {
            BigInt x = rnd.uniform(0, n - 1);
            BigInt y = x;
            BigInt c;
            do {
                c = rnd.uniform(1, n - 1);
            } while (c == n - 2);
            auto f = [&n, &c](const BigInt &x) { return (x * x + c) % n; };
            for (size_t iter_i = 0; !iter_times.has_value() || iter_i < iter_times.value(); ++iter_i) {
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
};

}
