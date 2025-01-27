#include <fstream>
#include "test_utils.h"
#include "except.h"

namespace BigPrimeLib {

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

}
