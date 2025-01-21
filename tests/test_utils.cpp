#include <fstream>
#include "test_utils.h"

namespace BigPrimeLib {

std::vector<BigInt> read_numbers(const char *path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Can't open file.");
    }
    std::vector<BigInt> primes;
    BigInt x;
    while (file >> x) {
        primes.push_back(x);
    }
    return primes;
}

}
