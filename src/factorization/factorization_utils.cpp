#include "factorization_utils.h"

namespace BigPrimeLib {

Factorizer::Factorizer() : prime_tester(nullptr) {}

Factorizer::Factorizer(const Factorizer &f)
    : prime_tester(f.prime_tester == nullptr ? nullptr : f.prime_tester->clone()) {}

Factorizer::Factorizer(const PrimeTester &prime_tester) : prime_tester(prime_tester.clone()) {}


std::vector<BigInt> Factorizer::factorization(const BigInt &n) {
    std::vector<BigInt> result;
    factorization_accumulate(n, result);
    return result;
}

void Factorizer::factorization_accumulate(const BigInt &n, std::vector<BigInt> &result) {
    if (n <= 1) {
        return;
    }
    if (prime_tester != nullptr && prime_tester->test(n) == PrimalityStatus::Prime) {
        result.push_back(n);
        return;
    }
    BigInt factor = find_factor(n).value_or(1);
    if (factor == 1 || factor == n) {
        if (n > 1) {
            result.push_back(n);
        }
        return;
    }
    factorization_accumulate(factor, result);
    factorization_accumulate(n / factor, result);
}


}
