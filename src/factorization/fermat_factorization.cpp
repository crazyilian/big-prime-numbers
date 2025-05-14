#include "fermat_factorization.h"

namespace BigPrimeLib {

FermatFactorizer::FermatFactorizer() {}

FermatFactorizer::FermatFactorizer(const PrimeTester &prime_tester) : prime_tester_(prime_tester) {}

PrimalityStatus FermatFactorizer::primality_test(const BigInt &n) {
    return prime_tester_ ? prime_tester_->test(n) : PrimalityStatus::Uncertain;
}

std::optional<BigInt> FermatFactorizer::find_factor(const BigInt &n) {
    if (n <= 3) {
        return std::nullopt;
    } else if (n % 2 == 0) {
        return 2;
    }
    BigInt a = Math::sqrt(n);
    BigInt b2 = a * a;
    if (b2 == n) {
        return a;
    }
    b2 -= n;
    while (true) {
        b2 += 2 * (a++) + 1;
        if (mpz_perfect_square_p(b2.backend().data())) {
            BigInt b = Math::sqrt(b2);
            BigInt factor = a - b;
            if (factor != 1 && factor != n) {
                return factor;
            }
            return std::nullopt;
        }
    }
}

}
