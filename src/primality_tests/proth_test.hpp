#pragma once

#include <boost/iterator/function_input_iterator.hpp>
#include "common.h"
#include "primality_utils.hpp"
#include "random.hpp"

namespace BigPrimeLib {

inline PrimalityStatus proth_test_base(const BigInt &n, const BigInt &base) {
    BigInt b = Math::powm(base, (n - 1) >> 1, n);
    if (b == n - 1) {
        return PrimalityStatus::Prime;
    } else if (b == 1) {
        return PrimalityStatus::Uncertain;
    } else {
        return PrimalityStatus::Composite;
    }
}

namespace detail {

    template<class Iterator>
    PrimalityStatus proth_test_iter(const BigInt &n, const Iterator &base_begin, const Iterator &base_end) {
        // n = k * 2^s + 1, k<2^s
        assert(Math::msb(n - 1) < Math::lsb(n - 1) * 2);
        for (auto it = base_begin; it != base_end; ++it) {
            BigInt base = *it;
            auto status = proth_test_base(n, base);
            if (status != PrimalityStatus::Uncertain) {
                return status;
            }
        }
        return PrimalityStatus::Uncertain;
    }

}

template<class Iterator>
class ProthPrimeTesterIter {
public:
    ProthPrimeTesterIter(const Iterator &begin, const Iterator &end, bool assume_composite = true)
        : begin_(begin), end_(end),
          on_uncertain_(assume_composite ? PrimalityStatus::Composite : PrimalityStatus::Uncertain) {}

    const PrimalityStatus &on_uncertain() const { return on_uncertain_; }

    PrimalityStatus test_raw(const BigInt &n) {
        return detail::proth_test_iter(n, begin_, end_);
    }

private:
    PrimalityStatus on_uncertain_;
    Iterator begin_, end_;
};

template<class RandomGenerator>
class ProthPrimeTester {
public:
    ProthPrimeTester(size_t times, const Random<RandomGenerator> &rnd, bool assume_composite = true)
        : times_(times), rnd_(rnd),
          on_uncertain_(assume_composite ? PrimalityStatus::Prime : PrimalityStatus::Uncertain) {}

    const PrimalityStatus &on_uncertain() const { return on_uncertain_; }

    PrimalityStatus test_raw(const BigInt &n) {
        std::function<BigInt()> f = [this, &n]() { return rnd_.uniform(2, n - 1); };
        return detail::proth_test_iter(n, Iterator(f, 0), Iterator(f, times_));
    }

private:
    using Iterator = boost::function_input_iterator<std::function<BigInt()>, size_t>;
    PrimalityStatus on_uncertain_;
    size_t times_;
    Random<RandomGenerator> rnd_;
};

}
