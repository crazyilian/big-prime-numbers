#include "sieve_eratosthenes.h"

namespace BigPrimeLib {

namespace {
    size_t distribution_of_primes_lower_bound(size_t x) {
        double lb = static_cast<double>(x) / (std::log(x + 3) - 1.1) + 5;
        return static_cast<size_t>(lb);
    }
}

SieveEratosthenes::SieveEratosthenes(size_t n) : n(n), least_div(n + 1) {
    primes.reserve(distribution_of_primes_lower_bound(n));
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

}
