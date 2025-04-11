#include <iostream>
#include "primality_tests/miller_rabin_test.hpp"
#include "common.h"
#include "except.h"
#include "random.hpp"

using BigPrimeLib::BigInt;
using BigPrimeLib::PrimalityStatus;
using BigPrimeLib::miller_rabin_prime_test;
using BigPrimeLib::Random;
namespace Math = BigPrimeLib::Math;
namespace Except = BigPrimeLib::Except;

int main() {
    try {
        Random rnd;
        BigInt x = Math::pow(BigInt(10), 100) + 267;
        if (miller_rabin_prime_test(x, 20, rnd) == PrimalityStatus::Composite) {
            std::cout << "Composite";
        } else {
            std::cout << "Probably prime";
        }
    } catch (...) {
        Except::react();
    }
}
