#pragma once

#include <folly/Poly.h>
#include "common.h"

namespace BigPrimeLib {

enum class PrimalityStatus {
    Prime,
    Composite,
    Uncertain,
    NotApplicable
};

inline const char *to_string(PrimalityStatus s) {
    switch (s) {
        case PrimalityStatus::Prime: return "Prime";
        case PrimalityStatus::Composite: return "Composite";
        case PrimalityStatus::Uncertain: return "Uncertain";
        case PrimalityStatus::NotApplicable: return "NotApplicable";
        default: return "[Unknown PrimalityStatus]";
    }
}

namespace _detail {

    struct IPrimeTester {
        template<class Base>
        struct Interface : Base {
            PrimalityStatus test(const BigInt &n) {
                if (n <= 1) {
                    return PrimalityStatus::NotApplicable;
                } else if (n <= 3) {
                    return PrimalityStatus::Prime;
                } else if (n % 2 == 0) {
                    return PrimalityStatus::Composite;
                } else {
                    auto status = test_raw(n);
                    return status == PrimalityStatus::Uncertain ? on_uncertain() : status;
                }
            }
            const PrimalityStatus &on_uncertain() const { return folly::poly_call<0>(*this); }
            PrimalityStatus test_raw(const BigInt &n) { return folly::poly_call<1>(*this, n); }
        };

        template<class T>
        using Members = folly::PolyMembers<&T::on_uncertain, &T::test_raw>;
    };

}

using PrimeTester = folly::Poly<_detail::IPrimeTester>;
using PrimeTesterRef = folly::Poly<_detail::IPrimeTester &>;

}
