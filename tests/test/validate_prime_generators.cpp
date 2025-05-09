#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "prime_generators/iterative_search.hpp"
#include "primality_tests/miller_rabin_test.hpp"
#include "prime_generators/maurer_method.hpp"

namespace BigPrimeLib::Test {

namespace {
    // search_next_prime

    TEST(search_next_prime, small_primes) {
        PrimeTester mrt = MillerRabinPrimeTester(20, Random());
        auto small_primes = read_numbers(Filenames::SmallPrimes);
        small_primes.resize(std::min(small_primes.size(), 1000ul));
        BigInt x = -10;
        for (const BigInt &p : small_primes) {
            for (; x <= p; ++x) {
                auto y = search_next_prime(x, mrt);
                EXPECT_TRUE(y == p) << "First prime after " << x << " is " << p << ", but found " << y;
            }
        }
    }

    // generate_prime_in_range

    void validate_generate_prime_in_range(const BigInt &l, const BigInt &r, Random<> &rnd, bool expect_prime,
                                          PrimeTester &prime_tester) {
        auto y_opt = generate_prime_in_range(l, r, rnd, prime_tester);
        if (expect_prime) {
            EXPECT_TRUE(y_opt.has_value()) << "No primes found in range " << "[" << l << ", " << r << "]";
            auto y = y_opt.value();
            EXPECT_TRUE(l <= y && y <= r) << y << " is not in range " << "[" << l << ", " << r << "]";
            auto status = prime_tester.test(y);
            EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found " << to_string(status) << " number " << y;
        } else {
            EXPECT_FALSE(y_opt.has_value()) << "Found prime " << y_opt.value() << " in range "
                    << "[" << l << ", " << r << "]";
        }
    }

    TEST(generate_random_prime_in_range, no_primes_range) {
        Random rnd;
        auto small_primes = read_numbers(Filenames::SmallPrimes);
        PrimeTester mrt = MillerRabinPrimeTester(20, rnd);
        for (size_t i = 1; i < small_primes.size(); ++i) {
            BigInt l = small_primes[i - 1] + 1;
            BigInt r = small_primes[i] - 1;
            if (l > r) {
                continue;
            }
            validate_generate_prime_in_range(l, r, rnd, false, mrt);
        }
    }

    TEST(generate_random_prime_in_range, small_range) {
        Random rnd;
        auto small_primes = read_numbers(Filenames::SmallPrimes);
        small_primes.resize(std::min(small_primes.size(), 3000ul));
        PrimeTester mrt = MillerRabinPrimeTester(20, rnd);
        for (const BigInt &p : small_primes) {
            BigInt l = p - rnd.uniform(0, 100);
            BigInt r = p + rnd.uniform(0, 100);
            validate_generate_prime_in_range(l, r, rnd, true, mrt);
        }
    }

    TEST(generate_random_prime_in_range, big_range) {
        Random rnd;
        BigInt C = Math::pow(BigInt(10), 300);
        PrimeTester mrt = MillerRabinPrimeTester(20, rnd);
        for (size_t i = 0; i < 10; ++i) {
            BigInt l = rnd.uniform(0, C);
            BigInt r = rnd.uniform(2 * l, 4 * C);
            validate_generate_prime_in_range(l, r, rnd, true, mrt);
        }
    }

    // generate_prime_maurer

    void validate_generate_prime_maurer(size_t bit_size, Random<> &rnd, PrimeTester &t) {
        auto y = generate_prime_maurer(bit_size, rnd);
        size_t y_bit_size = Math::msb(y);
        EXPECT_TRUE(y_bit_size != bit_size) << y << " has " << y_bit_size << " bits, but " << bit_size
                        << " bits required";
        auto status = t.test(y);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found " << to_string(status) << " number " << y;
    }

    TEST(generate_random_prime_maurer, small_bit_size) {
        Random rnd;
        PrimeTester mrt = MillerRabinPrimeTester(20, rnd);
        for (size_t bit_size = 2; bit_size < 300; ++bit_size) {
            validate_generate_prime_maurer(bit_size, rnd, mrt);
        }
    }

    TEST(generate_random_prime_maurer, big_bit_size) {
        Random rnd;
        PrimeTester mrt = MillerRabinPrimeTester(20, rnd);
        for (size_t bit_size = 300; bit_size <= 1000; bit_size += 100) {
            validate_generate_prime_maurer(bit_size, rnd, mrt);
        }
    }
    // generate_prime_maurer_with_cert

    void validate_generate_prime_maurer_with_cert(size_t bit_size, Random<> &rnd, PrimeTester &t) {
        auto y = generate_prime_maurer_with_cert(bit_size, rnd);
        size_t y_bit_size = Math::msb(y.p());
        EXPECT_TRUE(y_bit_size != bit_size) << y.p() << " has " << y_bit_size << " bits, but " << bit_size
                        << " bits required";
        EXPECT_TRUE(y.verify_assuming_prime_base()) << "Certificate is incorrect";
        auto status = t.test(y.p());
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found " << to_string(status) << " number " << y.p();

        for (const auto &cert_step : y.certificate()) {
            auto status2 = t.test(cert_step.p);
            EXPECT_TRUE(status2 == PrimalityStatus::Prime) << "Found " << to_string(status2) << " number "
                    << cert_step.p << " in certificate";
        }
    }

    TEST(generate_prime_maurer_with_cert, small_bit_size) {
        Random rnd;
        PrimeTester mrt = MillerRabinPrimeTester(20, rnd);
        for (size_t bit_size = 2; bit_size < 300; ++bit_size) {
            validate_generate_prime_maurer_with_cert(bit_size, rnd, mrt);
        }
    }

    TEST(generate_prime_maurer_with_cert, big_bit_size) {
        Random rnd;
        PrimeTester mrt = MillerRabinPrimeTester(20, rnd);
        for (size_t bit_size = 300; bit_size <= 1000; bit_size += 100) {
            validate_generate_prime_maurer_with_cert(bit_size, rnd, mrt);
        }
    }

}

}
