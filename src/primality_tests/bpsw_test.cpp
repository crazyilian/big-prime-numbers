#include "bpsw_test.h"
#include "fermat_test.hpp"
#include "miller_rabin_test.hpp"
#include "jacobi.hpp"
#include "lucas_test.h"

namespace BigPrimeLib {

namespace {

    PrimalityStatus lucas_test_wrapper(const BigInt &n, bool known_wieferich) {
        if (known_wieferich && (n == 1194649 || n == 12327121)) {
            // OEIS: A001220
            return PrimalityStatus::Composite;
        }

        int64_t D = 5;
        while (jacobi(D, n) != -1) {
            if (D > 0) {
                D = -D - 2;
            } else {
                D = -D + 2;
            }
            if (D == 17 && !known_wieferich && mpz_perfect_square_p(n.backend().data())) {
                return PrimalityStatus::Composite;
            }
        }

        return strong_lucas_test(n, 1, (1 - D) / 4, -1);
    }

}

PrimalityStatus bpsw_fermat_prime_test(const BigInt &n, bool known_wieferich) {
    if (auto status = test_leq_3(n); status != PrimalityStatus::Uncertain) {
        return status;
    } else if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    } else if (status = fermat_prime_test_base(n, 2); status != PrimalityStatus::Uncertain) {
        return status;
    }

    return lucas_test_wrapper(n, known_wieferich);
}

PrimalityStatus bpsw_prime_test(const BigInt &n, bool known_wieferich) {
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

    return lucas_test_wrapper(n, known_wieferich);
}

}
