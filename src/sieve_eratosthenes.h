#pragma once

#include "common.h"

namespace BigPrimeLib {

struct SieveEratosthenes {
    size_t n;
    std::vector<size_t> primes;
    std::vector<size_t> least_div;

    explicit SieveEratosthenes(size_t n);
};

}
