#pragma once

#include "common.h"
#include "primality_tests/primality_utils.hpp"

namespace BigPrimeLib {

namespace detail {

    struct IFactorizer {
        template<class Base>
        struct Interface : Base {
        private:
            void accumulate_factorization(const BigInt &n, std::vector<BigInt> &result) {
                if (n <= 1) {
                } else if (n <= 3 || primality_test(n) == PrimalityStatus::Prime) {
                    result.push_back(n);
                } else {
                    BigInt factor = find_factor(n).value_or(1);
                    if (factor == 1 || factor == n) {
                        if (n > 1) {
                            result.push_back(n);
                        }
                        return;
                    }
                    accumulate_factorization(factor, result);
                    accumulate_factorization(n / factor, result);
                }
            }

        public:
            std::vector<BigInt> factorize(const BigInt &n) {
                std::vector<BigInt> result;
                accumulate_factorization(n, result);
                return result;
            }
            std::optional<BigInt> find_factor(const BigInt &n) { return folly::poly_call<0>(*this, n); }
            PrimalityStatus primality_test(const BigInt &n) { return folly::poly_call<1>(*this, n); }
        };

        template<class T>
        using Members = folly::PolyMembers<&T::find_factor, &T::primality_test>;
    };

}

using Factorizer = folly::Poly<detail::IFactorizer>;
using FactorizerRef = folly::Poly<detail::IFactorizer &>;

}
