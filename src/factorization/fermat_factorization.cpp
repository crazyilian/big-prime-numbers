#include "fermat_factorization.h"
#include "factorization_utils.h"

namespace BigPrimeLib {

FermatFactorizer::FermatFactorizer() {}

FermatFactorizer::FermatFactorizer(const PrimeTester &prime_tester) : Factorizer(prime_tester) {}

std::unique_ptr<Factorizer> FermatFactorizer::clone() const {
    return std::make_unique<FermatFactorizer>(*this);
}

std::optional<BigInt> FermatFactorizer::find_factor(const BigInt &n) {
    assert(n > 0);
    if (n <= 3) {
        return std::nullopt;
    }
    if (n % 2 == 0) {
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
