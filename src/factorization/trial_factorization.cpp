#include "trial_factorization.h"
#include "factorization_utils.hpp"

namespace BigPrimeLib {

std::optional<BigInt> trial_find_factor(const BigInt &n) {
    assert(n > 0);
    if (n <= 3) {
        return std::nullopt;
    }
    if (n % 2 == 0) {
        return 2;
    }
    for (BigInt d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return d;
        }
    }
    return std::nullopt;
}

std::vector<BigInt> trial_factorization(const BigInt &n) {
    return full_factorization(n, trial_find_factor);
}

}
