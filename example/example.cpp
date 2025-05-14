#include <iostream>
#include "primality_tests/miller_rabin_test.hpp"
#include "common.h"
#include "except.h"
#include "random.hpp"

using BigPrimeLib::BigInt;
using BigPrimeLib::PrimalityStatus;
using BigPrimeLib::PrimeTester;
using BigPrimeLib::MillerRabinPrimeTester;
using BigPrimeLib::Random;
namespace Math = BigPrimeLib::Math;
namespace Except = BigPrimeLib::Except;

int main() {
    try {
        PrimeTester prime_tester = MillerRabinPrimeTester(20, Random());
        BigInt x = Math::pow(BigInt(10), 100) + 267;
        if (prime_tester.test(x) == PrimalityStatus::Composite) {
            std::cout << "Composite";
        } else {
            std::cout << "Probably prime";
        }
    } catch (...) {
        Except::react();
    }
}
