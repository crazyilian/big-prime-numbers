#pragma once

#include "common.h"

namespace BigPrimeLib {

namespace _detail {

    template<class FindFactorFunction, class... FindFactorArgs>
    void full_factorization_accumulate(const BigInt &n, std::vector<BigInt> &result,
                                       const FindFactorFunction &find_factor, FindFactorArgs &&... args) {
        if (n <= 1) {
            return;
        }
        std::optional<BigInt> factor = find_factor(n, std::forward<FindFactorArgs>(args)...);
        if (!factor.has_value()) {
            result.push_back(n);
            return;
        }
        full_factorization_accumulate(factor.value(), result, find_factor, std::forward<FindFactorArgs>(args)...);
        full_factorization_accumulate(n / factor.value(), result, find_factor, std::forward<FindFactorArgs>(args)...);
    }

}

template<class FindFactorFunction, class... FindFactorArgs>
std::vector<BigInt> full_factorization(const BigInt &n, const FindFactorFunction &find_factor,
                                       FindFactorArgs &&... args) {
    std::vector<BigInt> result;
    _detail::full_factorization_accumulate(n, result, find_factor, std::forward<FindFactorArgs>(args)...);
    return result;
}

}
