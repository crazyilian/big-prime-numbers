#include "bpsw_test.h"
#include "fermat_test.hpp"
#include "miller_rabin_test.hpp"
#include "jacobi.hpp"
#include "lucas_test.hpp"

namespace BigPrimeLib {

namespace {

    PrimalityStatus lucas_test_wrapper(const BigInt &n, bool known_wieferich, bool stronger_lucas) {
        if (known_wieferich && (n == 1194649 || n == 12327121)) {
            // OEIS: A001220
            return PrimalityStatus::Composite;
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
            if (D == 17 && !known_wieferich && mpz_perfect_square_p(n.backend().data())) {
                return PrimalityStatus::Composite;
            }
        }

        int64_t p = 1;
        int64_t q = (1 - D) / 4;
        if (D == 5) {
            p = q = 5;
        }

        if (!stronger_lucas) {
            return strong_lucas_test(n, p, q, -1);
        } else {
            return stronger_lucas_test(n, p, q, -1);
        }
    }

}

PrimalityStatus bpsw_fermat_prime_test(const BigInt &n, bool known_wieferich, bool stronger_lucas) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    } else if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    } else if (status = fermat_prime_test_base(n, 2); status != PrimalityStatus::Uncertain) {
        return status;
    }

    return lucas_test_wrapper(n, known_wieferich, stronger_lucas);
}

PrimalityStatus bpsw_miller_prime_test(const BigInt &n, bool known_wieferich, bool stronger_lucas) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    } else if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    }

    size_t s = Math::lsb(n - 1);
    BigInt t = n >> s;
    if (auto status = miller_rabin_prime_test_base(n, s, t, 2); status != PrimalityStatus::Uncertain) {
        return status;
    }

    return lucas_test_wrapper(n, known_wieferich, stronger_lucas);
}

}
