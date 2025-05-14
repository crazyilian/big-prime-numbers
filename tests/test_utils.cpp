#include <fstream>
#include "test_utils.h"
#include "except.h"

namespace BigPrimeLib::Test {

std::vector<BigInt> read_numbers(const char *path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw Except::CantOpenFileException(path);
    }
    std::vector<BigInt> primes;
    BigInt x;
    while (file >> x) {
        primes.push_back(x);
    }
    return primes;
}

std::vector<BigInt> read_all_big_primes() {
    auto big_primes = read_numbers(Filenames::BigPrimes);
    auto very_big_primes = read_numbers(Filenames::VeryBigPrimes);
    auto big_semiprimes = read_numbers(Filenames::BigSemiprimes);
    auto big_generalized_mersenne_primes = read_numbers(Filenames::BigGeneralizedMersennePrimes);
    auto big_proth_primes = read_numbers(Filenames::BigProthPrimes);

    big_primes.insert(big_primes.end(), very_big_primes.begin(), very_big_primes.end());
    big_primes.insert(big_primes.end(), big_semiprimes.begin(), big_semiprimes.end());
    for (size_t i = 0; i < big_generalized_mersenne_primes.size(); i += 2) {
        auto n = static_cast<uint64_t>(big_generalized_mersenne_primes[i]);
        auto k = big_generalized_mersenne_primes[i + 1];
        big_primes.push_back((k << n) - 1);
    }
    for (size_t i = 0; i < big_proth_primes.size(); i += 2) {
        auto n = static_cast<uint64_t>(big_proth_primes[i]);
        auto k = big_proth_primes[i + 1];
        big_primes.push_back((k << n) + 1);
    }
    std::sort(big_primes.begin(), big_primes.end());
    return big_primes;
}

}
