#include "bpsw_test.h"
#include "fermat_test.hpp"
#include "miller_rabin_test.hpp"
#include "jacobi.hpp"
#include "lucas_test.hpp"

namespace BigPrimeLib {

const std::vector<BigInt> kWieferichPrimes = {1093, 3511}; // OEIS: A001220

BPSWFeatures operator&(BPSWFeatures a, BPSWFeatures b) {
    return static_cast<BPSWFeatures>(static_cast<size_t>(a) & static_cast<size_t>(b));
}
BPSWFeatures &operator&=(BPSWFeatures &a, BPSWFeatures b) {
    return a = a & b;
}
BPSWFeatures operator|(BPSWFeatures a, BPSWFeatures b) {
    return static_cast<BPSWFeatures>(static_cast<size_t>(a) | static_cast<size_t>(b));
}
BPSWFeatures &operator|=(BPSWFeatures &a, BPSWFeatures b) {
    return a = a | b;
}

detail::BPSWPrimeTester::BPSWPrimeTester(BPSWFeatures features, bool assume_prime)
    : on_uncertain_(assume_prime ? PrimalityStatus::Prime : PrimalityStatus::Uncertain), features_(features) {}

const PrimalityStatus &detail::BPSWPrimeTester::on_uncertain() const {
    return on_uncertain_;
}

PrimalityStatus detail::BPSWPrimeTester::lucas_test_raw(const BigInt &n) const {
    if ((features_ & BPSWFeatures::KnownWieferich) != BPSWFeatures::None) {
        for (const auto &w : kWieferichPrimes) {
            if (n == w) {
                return PrimalityStatus::Prime;
            } else if (n % w == 0) {
                return PrimalityStatus::Composite;
            }
        }
    }

    int64_t D = 5;
    while (true) {
        int j = jacobi(D, n);
        if (j == -1) {
            break;
        } else if (j == 0) {
            return abs(D) == n ? PrimalityStatus::Prime : PrimalityStatus::Composite;
        }
        if (D > 0) {
            D = -D - 2;
        } else {
            D = -D + 2;
        }
        if (D == 17 && (features_ & BPSWFeatures::KnownWieferich) == BPSWFeatures::None
            && mpz_perfect_square_p(n.backend().data())) {
            return PrimalityStatus::Composite;
        }
    }

    int64_t p = 1;
    int64_t q = (1 - D) / 4;
    if (D == 5) {
        p = q = 5;
    }

    if ((features_ & BPSWFeatures::StrongerLucas) == BPSWFeatures::None) {
        return strong_lucas_test(n, p, q, -1);
    } else {
        return stronger_lucas_test(n, p, q, -1);
    }
}

PrimalityStatus BPSWMillerPrimeTester::test_raw(const BigInt &n) {
    assert(n >= 2);
    size_t s = Math::lsb(n - 1);
    BigInt t = n >> s;
    if (auto status = miller_rabin_test_base(n, s, t, 2); status != PrimalityStatus::Uncertain) {
        return status;
    }
    return lucas_test_raw(n);
}

PrimalityStatus BPSWFermatPrimeTester::test_raw(const BigInt &n) {
    assert(n >= 2);
    if (auto status = fermat_test_base(n, 2); status != PrimalityStatus::Uncertain) {
        return status;
    }
    return lucas_test_raw(n);
}

}
