#pragma once

#include <boost/iterator/function_input_iterator.hpp>
#include "common.h"
#include "primality_utils.hpp"
#include "random.hpp"

namespace BigPrimeLib {

const std::vector<BigInt> kMillerRabinDeterministicBasesU64 = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

inline PrimalityStatus miller_rabin_test_base(const BigInt &n, size_t s, const BigInt &t, const BigInt &base) {
    BigInt x = Math::powm(base, t, n);
    if (x == 1 || x == n - 1) {
        return PrimalityStatus::Uncertain;
    }
    for (size_t i = 0; i + 1 < s; ++i) {
        x = x * x % n;
        if (x == 1) {
            return PrimalityStatus::Composite;
        }
        if (x == n - 1) {
            return PrimalityStatus::Uncertain;
        }
    }
    return PrimalityStatus::Composite;
}

namespace _detail {

    template<class Iterator>
    PrimalityStatus miller_rabin_test_iter(const BigInt &n, const Iterator &base_begin, const Iterator &base_end) {
        size_t s = Math::lsb(n - 1);
        BigInt t = n >> s;
        for (auto it = base_begin; it != base_end; ++it) {
            if (miller_rabin_test_base(n, s, t, *it) == PrimalityStatus::Composite) {
                return PrimalityStatus::Composite;
            }
        }
        return PrimalityStatus::Uncertain;
    }

}

template<class Iterator>
class MillerRabinPrimeTesterIter {
public:
    MillerRabinPrimeTesterIter(const Iterator &begin, const Iterator &end, bool assume_prime = true)
        : begin_(begin), end_(end),
          on_uncertain_(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}

    PrimalityStatus test_raw(const BigInt &n) {
        return _detail::miller_rabin_test_iter(n, begin_, end_);
    }

    const PrimalityStatus &on_uncertain() const { return on_uncertain_; }

private:
    PrimalityStatus on_uncertain_;
    Iterator begin_, end_;
};

template<class RandomGenerator>
class MillerRabinPrimeTester {
public:
    MillerRabinPrimeTester(size_t times, const Random<RandomGenerator> &rnd, bool assume_prime = true)
        : times_(times), rnd_(rnd),
          on_uncertain_(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}

    PrimalityStatus test_raw(const BigInt &n) {
        std::function<BigInt()> f = [this, &n]() { return rnd_.uniform(2, n - 2); };
        return _detail::miller_rabin_test_iter(n, Iterator(f, 0), Iterator(f, times_));
    }

    const PrimalityStatus &on_uncertain() const { return on_uncertain_; }

private:
    using Iterator = boost::function_input_iterator<std::function<BigInt()>, size_t>;
    PrimalityStatus on_uncertain_;
    size_t times_;
    Random<RandomGenerator> rnd_;
};

}
