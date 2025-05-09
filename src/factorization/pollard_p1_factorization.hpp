#pragma once

#include "common.h"
#include "factorization_utils.h"
#include "random.hpp"

namespace BigPrimeLib {

template<class RandomGenerator>
class PollardP1Factorizer {
public:
    PollardP1Factorizer(const Random<RandomGenerator> &rnd, std::optional<size_t> restart_times,
                        std::optional<size_t> iter_times)
        : rnd_(rnd), restart_times_(restart_times), iter_times_(iter_times) {}

    PollardP1Factorizer(const PrimeTester &prime_tester, const Random<RandomGenerator> &rnd,
                        std::optional<size_t> restart_times, std::optional<size_t> iter_times)
        : prime_tester_(prime_tester), rnd_(rnd), restart_times_(restart_times), iter_times_(iter_times) {}

    PrimalityStatus primality_test(const BigInt &n) {
        return prime_tester_ ? prime_tester_->test(n) : PrimalityStatus::Uncertain;
    }

    std::optional<BigInt> find_factor(const BigInt &n) {
        if (n <= 3) {
            return std::nullopt;
        } else if (n % 2 == 0) {
            return 2;
        }
        for (size_t base_i = 0; !restart_times_.has_value() || base_i < restart_times_.value(); ++base_i) {
            BigInt a0 = rnd_.uniform(2, n - 2);
            if (auto d = run_once(n, a0); d.has_value()) {
                return d;
            }
        }
        return std::nullopt;
    }

private:
    std::optional<BigInt> run_once(const BigInt &n, const BigInt &a0) {
        BigInt a = a0;
        for (size_t iter_i = 0; !iter_times_.has_value() || iter_i < iter_times_.value(); ++iter_i) {
            a = Math::powm(a, iter_i + 1, n);
            if (a == 1) {
                break;
            } else if (iter_i > 20 && iter_i % 20 != 0) {
                continue;
            }
            BigInt g = Math::gcd(a - 1, n);
            if (g != 1) {
                return g;
            }
        }
        return std::nullopt;
    }

    std::optional<PrimeTester> prime_tester_;
    Random<RandomGenerator> rnd_;
    std::optional<size_t> restart_times_, iter_times_;
};

}
