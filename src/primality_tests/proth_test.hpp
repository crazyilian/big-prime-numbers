#pragma once

#include <boost/iterator/function_input_iterator.hpp>
#include "common.h"
#include "random.hpp"
#include "primality_utils.hpp"

namespace BigPrimeLib {

inline PrimalityStatus proth_prime_test_base(const BigInt &n, const BigInt &base) {
    BigInt b = Math::powm(base, (n - 1) >> 1, n);
    if (b == n - 1) {
        return PrimalityStatus::Prime;
    } else if (b == 1) {
        return PrimalityStatus::Uncertain;
    } else {
        return PrimalityStatus::Composite;
    }
}

template<class Iterator>
class ProthPrimeTesterIter : public PrimeTesterIter<Iterator> {
public:
    ProthPrimeTesterIter(bool assume_composite = true)
        : PrimeTesterIter<Iterator>(assume_composite ? PrimalityStatus::Composite : PrimalityStatus::Uncertain) {}
    ProthPrimeTesterIter(Iterator default_begin, Iterator default_end, bool assume_composite = true)
        : PrimeTesterIter<Iterator>(default_begin, default_end,
                                    assume_composite ? PrimalityStatus::Composite : PrimalityStatus::Uncertain) {}

    PrimalityStatus test_raw(const BigInt &n, Iterator base_begin, Iterator base_end) override {
        if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
            return status;
        }
        assert(Math::msb(n - 1) < Math::lsb(n - 1) * 2);
        for (auto it = base_begin; it != base_end; ++it) {
            BigInt base = *it;
            auto status = proth_prime_test_base(n, base);
            if (status != PrimalityStatus::Uncertain) {
                return status;
            }
        }
        return PrimalityStatus::Uncertain;
    }
};

template<class RandomGenerator>
class ProthPrimeTester : public PrimeTester {
public:
    size_t times;
    Random<RandomGenerator> rnd;

public:
    ProthPrimeTester(size_t times, Random<RandomGenerator> rnd, bool assume_composite = true)
        : PrimeTester(assume_composite ? PrimalityStatus::Composite : PrimalityStatus::Uncertain),
          times(times), rnd(rnd) {}

    PrimalityStatus test_raw(const BigInt &n) override {
        // n = k*2^s+1, k<2^s
        assert(n > 2);
        if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
            return status;
        }
        std::function<BigInt()> f = [this, &n]() { return rnd.uniform(2, n - 1); };
        return tester_iter_.test(n, Iterator(f, 0), Iterator(f, times));
    }

private:
    using Iterator = boost::function_input_iterator<std::function<BigInt()>, size_t>;
    ProthPrimeTesterIter<Iterator> tester_iter_;
};

}
