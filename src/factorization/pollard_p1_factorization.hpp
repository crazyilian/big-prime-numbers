#pragma once

#include "common.h"
#include "factorization_utils.h"
#include "random.hpp"

namespace BigPrimeLib {

template<class RandomGenerator>
class PollardP1Factorizer : public Factorizer {
public:
    Random<RandomGenerator> rnd;
    std::optional<size_t> base_times, iter_times;

public:
    PollardP1Factorizer(Random<RandomGenerator> rnd, std::optional<size_t> base_times, std::optional<size_t> iter_times)
        : rnd(rnd), base_times(base_times), iter_times(iter_times) {}

    PollardP1Factorizer(const PrimeTester &prime_tester, Random<RandomGenerator> rnd,
                        std::optional<size_t> base_times, std::optional<size_t> iter_times)
        : Factorizer(prime_tester), rnd(rnd), base_times(base_times), iter_times(iter_times) {}

    std::unique_ptr<Factorizer> clone() const override {
        return std::make_unique<PollardP1Factorizer>(*this);
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
            BigInt a = rnd.uniform(2, n - 2);
            for (size_t iter_i = 0; !iter_times.has_value() || iter_i < iter_times.value(); ++iter_i) {
                a = Math::powm(a, iter_i + 1, n);
                if (a == 1) {
                    break;
                }
                if (iter_i < 20 || iter_i % 20 == 0) {
                    BigInt g = Math::gcd(a - 1, n);
                    if (g != 1) {
                        return g;
                    }
                }
            }
        }
        return std::nullopt;
    }
};

}
