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

namespace _detail {

    template<class Iterator>
    PrimalityStatus fermat_test_iter(const BigInt &n, const Iterator &base_begin, const Iterator &base_end) {
        for (auto it = base_begin; it != base_end; ++it) {
            if (fermat_test_base(n, *it) == PrimalityStatus::Composite) {
                return PrimalityStatus::Composite;
            }
        }
        return PrimalityStatus::Uncertain;
    }

}

template<class Iterator>
class FermatPrimeTesterIter {
public:
    FermatPrimeTesterIter(const Iterator &begin, const Iterator &end, bool assume_prime = true)
        : begin_(begin), end_(end),
          on_uncertain_(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}

    PrimalityStatus test_raw(const BigInt &n) {
        return _detail::fermat_test_iter(n, begin_, end_);
    }

    const PrimalityStatus &on_uncertain() const { return on_uncertain_; }

private:
    PrimalityStatus on_uncertain_;
    Iterator begin_, end_;
};

template<class RandomGenerator>
class FermatPrimeTester {
public:
    FermatPrimeTester(size_t times, const Random<RandomGenerator> &rnd, bool assume_prime = true)
        : times_(times), rnd_(rnd),
          on_uncertain_(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}

    PrimalityStatus test_raw(const BigInt &n) {
        std::function<BigInt()> f = [this, &n]() { return rnd_.uniform(2, n - 1); };
        return _detail::fermat_test_iter(n, Iterator(f, 0), Iterator(f, times_));
    }

    const PrimalityStatus &on_uncertain() const { return on_uncertain_; }

private:
    using Iterator = boost::function_input_iterator<std::function<BigInt()>, size_t>;
    PrimalityStatus on_uncertain_;
    size_t times_;
    Random<RandomGenerator> rnd_;
};

}
