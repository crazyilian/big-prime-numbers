#include <iostream>
#include <primality_tests/miller_rabin_prime_test.hpp>

#include "primality_tests/fermat_test.hpp"

using BigPrimeLib::BigInt;
using BigPrimeLib::PrimalityStatus;
using BigPrimeLib::miller_rabin_prime_test;

int main() {
    boost::random::mt19937 rng(42);
    BigInt x = boost::multiprecision::pow(BigInt(10), 100) + 267;

    if (miller_rabin_prime_test(x, 20, rng) == PrimalityStatus::Composite) {
        std::cout << "Composite";
    } else {
        std::cout << "Probably prime";
    }
}
