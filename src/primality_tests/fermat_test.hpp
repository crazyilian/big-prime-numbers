#pragma once

#include <boost/iterator/function_input_iterator.hpp>
#include "common.h"
#include "primality_utils.hpp"
#include "random.hpp"

namespace BigPrimeLib {

inline PrimalityStatus fermat_test_base(const BigInt &n, const BigInt &base) {
    if (Math::powm(base, n - 1, n) == 1) {
        return PrimalityStatus::Uncertain;
    } else {
        return PrimalityStatus::Composite;
    }
}

template<class Iterator>
class FermatPrimeTesterIter : public PrimeTesterIter<Iterator> {
public:
    explicit FermatPrimeTesterIter(bool assume_prime = true)
        : PrimeTesterIter<Iterator>(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}
    FermatPrimeTesterIter(Iterator default_begin, Iterator default_end, bool assume_prime = true)
        : PrimeTesterIter<Iterator>(default_begin, default_end,
                                    assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}

    PrimalityStatus test_raw(const BigInt &n, Iterator base_begin, Iterator base_end) override {
        if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
            return status;
        }
        for (auto it = base_begin; it != base_end; ++it) {
            if (fermat_test_base(n, *it) == PrimalityStatus::Composite) {
                return PrimalityStatus::Composite;
            }
        }
        return PrimalityStatus::Uncertain;
    }
};

template<class RandomGenerator>
class FermatPrimeTester : public PrimeTester {
public:
    size_t times;
    Random<RandomGenerator> rnd;

public:
    FermatPrimeTester(size_t times, Random<RandomGenerator> rnd, bool assume_prime = true)
        : PrimeTester(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain), times(times), rnd(rnd) {}

    PrimalityStatus test_raw(const BigInt &n) override {
        if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
            return status;
        }
        std::function<BigInt()> f = [this, &n]() { return rnd.uniform(2, n - 1); };
        return tester_iter_.test(n, Iterator(f, 0), Iterator(f, times));
    }

private:
    using Iterator = boost::function_input_iterator<std::function<BigInt()>, size_t>;
    FermatPrimeTesterIter<Iterator> tester_iter_;
};

}
