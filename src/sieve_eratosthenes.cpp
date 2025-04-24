#include "sieve_eratosthenes.h"

namespace BigPrimeLib {

SieveEratosthenes::SieveEratosthenes(size_t n) : n(n), least_div(n + 1) {
    primes.reserve(static_cast<size_t>(static_cast<double>(n) / (std::log(n + 3) - 1.1)) + 5); // >= pi(n)
    for (size_t x = 2; x <= n; ++x) {
        if (least_div[x] == 0) {
            least_div[x] = x;
            primes.push_back(x);
        }
        for (size_t i = 0; i < primes.size() && primes[i] <= least_div[x] && primes[i] * x <= n; ++i) {
            least_div[primes[i] * x] = primes[i];
        }
    }
}

PrimalityStatus SieveEratosthenes::is_prime(size_t x) const {
    if (x <= 1) {
        return PrimalityStatus::NotApplicable;
    } else if (x > n) {
        return PrimalityStatus::Uncertain;
    } else if (least_div[x] == x) {
        return PrimalityStatus::Prime;
    } else {
        return PrimalityStatus::Composite;
    }
}


}
