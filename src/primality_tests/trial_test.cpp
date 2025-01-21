#include "trial_test.h"
#include "primality_status.h"

namespace BigPrimeLib {

PrimalityStatus trial_prime_test(const BigInt &n) {
    if (n <= 1) {
        return PrimalityStatus::NotApplicable;
    }
    if (n <= 3) {
        return PrimalityStatus::Prime;
    }
    if (n % 2 == 0) {
        return PrimalityStatus::Composite;
    }
    for (BigInt d = 3; d * d <= n; d += 2) {
        if (n % d == 0) {
            return PrimalityStatus::Composite;
        }
    }
    return PrimalityStatus::Prime;
}


}
