#include <iostream>
#include "primality_tests/miller_rabin_prime_test.hpp"
#include "common.hpp"

using BigPrimeLib::BigInt;
using BigPrimeLib::PrimalityStatus;
using BigPrimeLib::miller_rabin_prime_test;
using BigPrimeLib::Random;

int main() {
    Random rnd;
    BigInt x = BigPrimeLib::Math::pow(BigInt(10), 100) + 267;

    if (miller_rabin_prime_test(x, 20, rnd) == PrimalityStatus::Composite) {
        std::cout << "Composite";
    } else {
        std::cout << "Probably prime";
    }
}
