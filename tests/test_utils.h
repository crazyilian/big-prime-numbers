#pragma once

#include "common.h"

namespace BigPrimeLib {

std::vector<BigInt> read_numbers(const char *path);

namespace Filenames {
    inline constexpr auto SmallPrimes = "data/first_10000_primes.txt";
    inline constexpr auto BigPrimes = "data/random_big_primes.txt";
    inline constexpr auto SmallCarmichael = "data/carmichael_less_10000th_prime.txt";
    inline constexpr auto MersennePrimes = "data/mersenne_primes.txt";
    inline constexpr auto BigGeneralizedMersennePrimes = "data/random_big_generalized_mersenne_primes.txt";
}

}
