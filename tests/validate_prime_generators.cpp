#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "prime_generators/iterative_search.hpp"
#include "primality_tests/miller_rabin_test.hpp"
#include "primality_tests/bpsw_test.h"
#include "prime_generators/maurer_method.hpp"

namespace BigPrimeLib {

// search_next_prime

TEST(search_next_prime, small_primes) {
    Random rnd;
    auto small_primes = read_numbers(Filenames::SmallPrimes);
    small_primes.resize(std::min(small_primes.size(), 1000ul));
    BigInt x = -10;
    for (const BigInt &p : small_primes) {
        for (; x <= p; ++x) {
            auto y = search_next_prime(x, miller_rabin_prime_test_assume_prime<decltype(rnd)>, 20, rnd);
            EXPECT_TRUE(y == p) << "First prime after " << x << " is " << p << ", but found " << y;
        }
    }
}

// generate_prime_in_range

void validate_generate_prime_in_range(const BigInt &l, const BigInt &r, Random<> &rnd) {
    auto y = generate_prime_in_range(l, r, rnd, miller_rabin_prime_test_assume_prime<decltype(rnd)>, 20, rnd);
    EXPECT_TRUE(l <= y && y <= r) << y << " is not in range " << "[" << l << ", " << r << "]";
    auto status = miller_rabin_prime_test_assume_prime(y, 20, rnd);
    EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found " << to_string(status) << " number " << y;
}

TEST(generate_random_prime_in_range, small_range) {
    Random rnd;
    auto small_primes = read_numbers(Filenames::SmallPrimes);
    small_primes.resize(std::min(small_primes.size(), 3000ul));
    for (const BigInt &p : small_primes) {
        BigInt l = p - rnd.uniform(0, 100);
        BigInt r = p + rnd.uniform(0, 100);
        validate_generate_prime_in_range(l, r, rnd);
    }
}

TEST(generate_random_prime_in_range, big_range) {
    Random rnd;
    Random rnd_test;
    BigInt C = Math::pow(BigInt(10), 300);
    for (size_t i = 0; i < 10; ++i) {
        BigInt l = rnd_test.uniform(0, C);
        BigInt r = rnd_test.uniform(2 * l, 4 * C);
        validate_generate_prime_in_range(l, r, rnd);
    }
}

// generate_prime_maurer

void validate_generate_prime_maurer(size_t bit_size, Random<> &rnd) {
    auto y = generate_prime_maurer(bit_size, rnd);
    size_t y_bit_size = Math::msb(y);
    EXPECT_TRUE(y_bit_size != bit_size) << y << " has " << y_bit_size << " bits, but " << bit_size
                    << " bits required";
    auto status = miller_rabin_prime_test_assume_prime(y, 20, rnd);
    EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found " << to_string(status) << " number " << y;
}

TEST(generate_random_prime_maurer, small_bit_size) {
    Random rnd;
    for (size_t bit_size = 2; bit_size < 300; ++bit_size) {
        validate_generate_prime_maurer(bit_size, rnd);
    }
}

TEST(generate_random_prime_maurer, big_bit_size) {
    Random rnd;
    for (size_t bit_size = 300; bit_size <= 1000; bit_size += 100) {
        validate_generate_prime_maurer(bit_size, rnd);
    }
}
// generate_prime_maurer_with_cert

void validate_generate_prime_maurer_with_cert(size_t bit_size, Random<> &rnd) {
    auto y = generate_prime_maurer_with_cert(bit_size, rnd);
    size_t y_bit_size = Math::msb(y.p);
    EXPECT_TRUE(y_bit_size != bit_size) << y.p << " has " << y_bit_size << " bits, but " << bit_size
                    << " bits required";
    EXPECT_TRUE(y.verify_assuming_prime_base()) << "Certificate is incorrect";
    auto status = miller_rabin_prime_test_assume_prime(y.p, 20, rnd);
    EXPECT_TRUE(status == PrimalityStatus::Prime) << "Found " << to_string(status) << " number " << y.p;

    for (auto c = y.cert.get(); c != nullptr; c = c->q.cert.get()) {
        auto status2 = miller_rabin_prime_test_assume_prime(c->q.p, 20, rnd);
        EXPECT_TRUE(status2 == PrimalityStatus::Prime) << "Found " << to_string(status2) << " number " << c->q.p
                << " in certificate";
    }
}

TEST(generate_prime_maurer_with_cert, small_bit_size) {
    Random rnd;
    for (size_t bit_size = 2; bit_size < 300; ++bit_size) {
        validate_generate_prime_maurer_with_cert(bit_size, rnd);
    }
}

TEST(generate_prime_maurer_with_cert, big_bit_size) {
    Random rnd;
    for (size_t bit_size = 300; bit_size <= 1000; bit_size += 100) {
        validate_generate_prime_maurer_with_cert(bit_size, rnd);
    }
}


}
