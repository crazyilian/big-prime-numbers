#pragma once

#include "common.h"
#include "primality_tests/primality_utils.hpp"

namespace BigPrimeLib {

class SieveEratosthenes {
public:
    size_t n;
    std::vector<size_t> primes;
    std::vector<size_t> least_prime;

public:
    explicit SieveEratosthenes(size_t n);

    PrimalityStatus is_prime(size_t x) const;
};

}
