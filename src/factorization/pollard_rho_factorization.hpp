#pragma once

#include "common.h"
#include "factorization_utils.h"
#include "random.hpp"

namespace BigPrimeLib {

template<class RandomGenerator>
class PollardRhoFactorizer {
public:
    PollardRhoFactorizer(const Random<RandomGenerator> &rnd, std::optional<size_t> restart_times,
                         std::optional<size_t> iter_times)
        : rnd_(rnd), restart_times_(restart_times), iter_times_(iter_times) {}

    PollardRhoFactorizer(const PrimeTester &prime_tester, const Random<RandomGenerator> &rnd,
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
            BigInt x0 = rnd_.uniform(0, n - 1);
            BigInt c;
            do {
                c = rnd_.uniform(1, n - 1);
            } while (c == n - 2);
            if (auto d = run_once(n, c, x0); d.has_value()) {
                return d;
            }
        }
        return std::nullopt;
    }

private:
    std::optional<BigInt> run_once(const BigInt &n, const BigInt &c, const BigInt &x0) const {
        BigInt x = x0;
        BigInt y = x0;
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
        return std::nullopt;
    }

    std::optional<PrimeTester> prime_tester_;
    Random<RandomGenerator> rnd_;
    std::optional<size_t> restart_times_, iter_times_;
};

}
