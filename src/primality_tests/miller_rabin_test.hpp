#pragma once

#include <boost/iterator/function_input_iterator.hpp>
#include "common.h"
#include "random.hpp"
#include "primality_utils.hpp"

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

template<class Iterator>
class MillerRabinPrimeTesterIter : public PrimeTesterIter<Iterator> {
public:
    explicit MillerRabinPrimeTesterIter(bool assume_prime = true)
        : PrimeTesterIter<Iterator>(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}
    MillerRabinPrimeTesterIter(Iterator default_begin, Iterator default_end, bool assume_prime = true)
        : PrimeTesterIter<Iterator>(default_begin, default_end,
                                    assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}

    std::unique_ptr<PrimeTester> clone() const override {
        return std::make_unique<MillerRabinPrimeTesterIter>(*this);
    }

    PrimalityStatus test_raw(const BigInt &n, Iterator base_begin, Iterator base_end) override {
        if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
            return status;
        }
        size_t s = Math::lsb(n - 1);
        BigInt t = n >> s;
        for (auto it = base_begin; it != base_end; ++it) {
            if (miller_rabin_test_base(n, s, t, *it) == PrimalityStatus::Composite) {
                return PrimalityStatus::Composite;
            }
        }
        return PrimalityStatus::Uncertain;
    }
};


template<class RandomGenerator>
class MillerRabinPrimeTester : public PrimeTester {
public:
    size_t times;
    Random<RandomGenerator> rnd;

public:
    MillerRabinPrimeTester(size_t times, Random<RandomGenerator> rnd, bool assume_prime = true)
        : PrimeTester(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain), times(times), rnd(rnd) {}

    std::unique_ptr<PrimeTester> clone() const override {
        return std::make_unique<MillerRabinPrimeTester>(*this);
    }

    PrimalityStatus test_raw(const BigInt &n) override {
        if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
            return status;
        }
        std::function<BigInt()> f = [this, &n]() { return rnd.uniform(2, n - 2); };
        return tester_iter_.test(n, Iterator(f, 0), Iterator(f, times));
    }

private:
    using Iterator = boost::function_input_iterator<std::function<BigInt()>, size_t>;
    MillerRabinPrimeTesterIter<Iterator> tester_iter_;
};

}
