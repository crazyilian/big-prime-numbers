#include <unordered_set>
#include <gtest/gtest.h>
#include "common.h"
#include "test_utils.h"
#include "primality_tests/bpsw_test.cpp"
#include "primality_tests/trial_test.h"
#include "primality_tests/fermat_test.hpp"
#include "primality_tests/miller_rabin_test.hpp"
#include "primality_tests/lucas_lehmer_test.h"
#include "primality_tests/lucas_lehmer_riesel_test.h"
#include "primality_tests/proth_test.hpp"

namespace BigPrimeLib {

template<class PrimalityTestFunction, class... PrimalityTestArgs>
void validate_on_small_primes(const std::unordered_set<BigInt> &skip, const PrimalityTestFunction &prime_test,
                              PrimalityTestArgs &&... args) {
    std::vector<BigInt> small_primes = read_numbers(Filenames::SmallPrimes);
    std::unordered_set<BigInt> small_primes_set(small_primes.begin(), small_primes.end());

    for (BigInt x = 2; x <= small_primes.back(); ++x) {
        if (skip.contains(x)) {
            continue;
        }
        bool is_prime = small_primes_set.contains(x);
        PrimalityStatus out = prime_test(x, std::forward<PrimalityTestArgs>(args)...);
        if (is_prime) {
            EXPECT_TRUE(out == PrimalityStatus::Prime)
                    << "Prime " << x << " marked as " << to_string(out);
        } else {
            EXPECT_TRUE(out == PrimalityStatus::Composite)
                    << "Composite " << x << " marked as " << to_string(out);
        }
    }
}

template<class PrimalityTestFunction, class... PrimalityTestArgs>
void validate_on_big_primes(const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
    std::vector<BigInt> big_primes = read_numbers(Filenames::BigPrimes);

    for (const BigInt &x : big_primes) {
        PrimalityStatus out = prime_test(x, std::forward<PrimalityTestArgs>(args)...);
        EXPECT_TRUE(out == PrimalityStatus::Prime) << "Prime " << x << " marked as " << to_string(out);
    }
}

template<class PrimalityTestFunction, class... PrimalityTestArgs>
void validate_on_big_product_two_primes(const PrimalityTestFunction &prime_test, PrimalityTestArgs &&... args) {
    std::vector<BigInt> big_primes = read_numbers(Filenames::BigPrimes);

    for (size_t i = 0; i < big_primes.size(); ++i) {
        for (size_t j = i; j < big_primes.size(); ++j) {
            BigInt x = big_primes[i] * big_primes[j];
            PrimalityStatus out = prime_test(x, std::forward<PrimalityTestArgs>(args)...);
            EXPECT_TRUE(out == PrimalityStatus::Composite)
                << "Composite " << big_primes[i] << "*" << big_primes[j] << " marked as " << to_string(out);
        }
    }
}

// trial_prime_test

TEST(trial_prime_test, small_primes) {
    validate_on_small_primes({}, trial_prime_test);
}

// fermat_prime_test

TEST(fermat_prime_test, small_primes) {
    Random rnd;
    auto carmichael = read_numbers(Filenames::SmallCarmichael);
    std::unordered_set<BigInt> skip(carmichael.begin(), carmichael.end());
    validate_on_small_primes(skip, fermat_prime_test_assume_prime<>, 10, rnd);
}

TEST(fermat_prime_test, big_primes) {
    Random rnd;
    validate_on_big_primes(fermat_prime_test_assume_prime<>, 10, rnd);
}

TEST(fermat_prime_test, big_product_two_primes) {
    Random rnd;
    validate_on_big_product_two_primes(fermat_prime_test_assume_prime<>, 10, rnd);
}

// fermat_prime_test_iter

TEST(fermat_prime_test_iter, small_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    auto carmichael = read_numbers(Filenames::SmallCarmichael);
    std::unordered_set<BigInt> skip(carmichael.begin(), carmichael.end());
    skip.insert(bases.begin(), bases.end());
    validate_on_small_primes(skip, fermat_prime_test_iter_assume_prime<std::vector<BigInt>::iterator>, bases.begin(),
                             bases.end());
}

TEST(fermat_prime_test_iter, big_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_primes(fermat_prime_test_iter_assume_prime<std::vector<BigInt>::iterator>, bases.begin(),
                           bases.end());
}

TEST(fermat_prime_test_iter, big_product_two_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_product_two_primes(fermat_prime_test_iter_assume_prime<std::vector<BigInt>::iterator>,
                                       bases.begin(), bases.end());
}

// miller_rabin_prime_test

TEST(miller_rabin_prime_test, small_primes) {
    Random rnd;
    validate_on_small_primes({}, miller_rabin_prime_test_assume_prime<>, 10, rnd);
}

TEST(miller_rabin_prime_test, big_primes) {
    Random rnd;
    validate_on_big_primes(miller_rabin_prime_test_assume_prime<>, 10, rnd);
}

TEST(miller_rabin_prime_test, big_product_two_primes) {
    Random rnd;
    validate_on_big_product_two_primes(miller_rabin_prime_test_assume_prime<>, 10, rnd);
}

// miller_rabin_prime_test_iter

TEST(miller_rabin_prime_test_iter, small_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    std::unordered_set<BigInt> skip(bases.begin(), bases.end());
    skip.insert(12);
    validate_on_small_primes(skip, miller_rabin_prime_test_iter_assume_prime<std::vector<BigInt>::iterator>,
                             bases.begin(), bases.end());
}

TEST(miller_rabin_prime_test_iter, big_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_primes(miller_rabin_prime_test_iter_assume_prime<std::vector<BigInt>::iterator>, bases.begin(),
                           bases.end());
}

TEST(miller_rabin_prime_test_iter, big_product_two_primes) {
    std::vector<BigInt> bases(10);
    std::iota(bases.begin(), bases.end(), 2);
    validate_on_big_product_two_primes(miller_rabin_prime_test_iter_assume_prime<std::vector<BigInt>::iterator>,
                                       bases.begin(), bases.end());
}

// lucas_lehmer_prime_test

TEST(lucas_lehmer_prime_test, small_mersenne) {
    auto small_primes = read_numbers(Filenames::SmallPrimes);
    auto mersenne_primes = read_numbers(Filenames::MersennePrimes);
    std::set<BigInt> mersenne_primes_set(mersenne_primes.begin(), mersenne_primes.end());
    for (const BigInt &p : small_primes) {
        if (p > 3300) {
            break;
        }
        BigInt N = (BigInt(1) << static_cast<uint64_t>(p)) - 1;
        auto status = lucas_lehmer_prime_test(N);
        if (mersenne_primes_set.contains(p)) {
            EXPECT_TRUE(status == PrimalityStatus::Prime) << "Prime 2^" << p << "-1 marked as " << to_string(status);
        } else {
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Prime 2^" << p << "-1 marked as "
                    << to_string(status);
        }
    }
}

// lucas_lehmer_riesel_prime_test

TEST(lucas_lehmer_riesel_prime_test, small_generalized_mersenne) {
    for (uint64_t n = 1; n < 100; ++n) {
        for (uint64_t k = 1; k < 200 && (n >= 10 || k < (1 << n)); ++k) {
            // 2^n > k
            BigInt N = (BigInt(k) << n) - 1;
            auto status_lucas = lucas_lehmer_riesel_prime_test(N);
            Random rnd;
            auto status_miller = miller_rabin_prime_test_assume_prime(N, 20, rnd);
            EXPECT_TRUE(status_lucas == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                    << to_string(status_lucas);

        }
    }
}

TEST(lucas_lehmer_riesel_prime_test, big_generalized_mersenne_random) {
    Random rnd(42);
    for (size_t i = 0; i < 10; ++i) {
        auto n = static_cast<uint64_t>(rnd.uniform(1, 5000));
        auto k = rnd.uniform(1, (BigInt(1) << n) - 1);

        BigInt N = (k << n) - 1;
        auto status_lucas = lucas_lehmer_riesel_prime_test(N);
        auto status_miller = miller_rabin_prime_test_assume_prime(N, 20, rnd);
        EXPECT_TRUE(status_lucas == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                << to_string(status_lucas);
    }
}

TEST(lucas_lehmer_riesel_prime_test, big_generalized_mersenne_primes) {
    auto numbers = read_numbers(Filenames::BigGeneralizedMersennePrimes);
    for (size_t i = 0; i < numbers.size(); i += 2) {
        auto n = static_cast<uint64_t>(numbers[i]);
        auto k = numbers[i + 1];
        BigInt N = (k << n) - 1;
        auto status = lucas_lehmer_riesel_prime_test(N);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Prime " << k << " * 2^" << n << " - 1 marked as "
                << to_string(status);
    }
}


// bpsw_miller_prime_test

TEST(bpsw_miller_prime_test, small_primes) {
    validate_on_small_primes({}, bpsw_miller_prime_test_assume_prime, true, false);
}

TEST(bpsw_miller_prime_test, big_primes) {
    validate_on_big_primes(bpsw_miller_prime_test_assume_prime, true, false);
}

TEST(bpsw_miller_prime_test, big_product_two_primes) {
    validate_on_big_product_two_primes(bpsw_miller_prime_test_assume_prime, true, false);
}

TEST(bpsw_miller_prime_test, small_squares) {
    for (BigInt x = 2; x < 300000; ++x) {
        auto status = bpsw_miller_prime_test_assume_prime(x * x, true, false);
        EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(status);
    }
}


// bpsw_miller_prime_test_no_wieferich

TEST(bpsw_miller_prime_test_no_wieferich, small_primes) {
    validate_on_small_primes({}, bpsw_miller_prime_test_assume_prime, false, false);
}

TEST(bpsw_miller_prime_test_no_wieferich, big_primes) {
    validate_on_big_primes(bpsw_miller_prime_test_assume_prime, false, false);
}

TEST(bpsw_miller_prime_test_no_wieferich, big_product_two_primes) {
    validate_on_big_product_two_primes(bpsw_miller_prime_test_assume_prime, false, false);
}

TEST(bpsw_miller_prime_test_no_wieferich, small_squares) {
    for (BigInt x = 2; x < 300000; ++x) {
        auto status = bpsw_miller_prime_test_assume_prime(x * x, false, false);
        EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(status);
    }
}

// bpsw_miller_prime_test_stronger_lucas

TEST(bpsw_miller_prime_test_stronger_lucas, small_primes) {
    validate_on_small_primes({}, bpsw_miller_prime_test_assume_prime, true, true);
}

TEST(bpsw_miller_prime_test_stronger_lucas, big_primes) {
    validate_on_big_primes(bpsw_miller_prime_test_assume_prime, true, true);
}

TEST(bpsw_miller_prime_test_stronger_lucas, big_product_two_primes) {
    validate_on_big_product_two_primes(bpsw_miller_prime_test_assume_prime, true, true);
}

TEST(bpsw_miller_prime_test_stronger_lucas, small_squares) {
    for (BigInt x = 2; x < 300000; ++x) {
        auto status = bpsw_miller_prime_test_assume_prime(x * x, true, true);
        EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(status);
    }
}

// bpsw_fermat_prime_test

TEST(bpsw_fermat_prime_test, small_primes) {
    validate_on_small_primes({}, bpsw_fermat_prime_test_assume_prime, true, false);
}

TEST(bpsw_fermat_prime_test, big_primes) {
    validate_on_big_primes(bpsw_fermat_prime_test_assume_prime, true, false);
}

TEST(bpsw_fermat_prime_test, big_product_two_primes) {
    validate_on_big_product_two_primes(bpsw_fermat_prime_test_assume_prime, true, false);
}

TEST(bpsw_fermat_prime_test, small_squares) {
    for (BigInt x = 2; x < 300000; ++x) {
        auto status = bpsw_fermat_prime_test_assume_prime(x * x, true, false);
        EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(status);
    }
}

// bpsw_fermat_prime_test_no_wieferich

TEST(bpsw_fermat_prime_test_no_wieferich, small_primes) {
    validate_on_small_primes({}, bpsw_fermat_prime_test_assume_prime, false, false);
}

TEST(bpsw_fermat_prime_test_no_wieferich, big_primes) {
    validate_on_big_primes(bpsw_fermat_prime_test_assume_prime, false, false);
}

TEST(bpsw_fermat_prime_test_no_wieferich, big_product_two_primes) {
    validate_on_big_product_two_primes(bpsw_fermat_prime_test_assume_prime, false, false);
}

TEST(bpsw_fermat_prime_test_no_wieferich, small_squares) {
    for (BigInt x = 2; x < 300000; ++x) {
        auto status = bpsw_fermat_prime_test_assume_prime(x * x, false, false);
        EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(status);
    }
}

// bpsw_fermat_prime_test_stronger_lucas

TEST(bpsw_fermat_prime_test_stronger_lucas, small_primes) {
    validate_on_small_primes({}, bpsw_fermat_prime_test_assume_prime, true, true);
}

TEST(bpsw_fermat_prime_test_stronger_lucas, big_primes) {
    validate_on_big_primes(bpsw_fermat_prime_test_assume_prime, true, true);
}

TEST(bpsw_fermat_prime_test_stronger_lucas, big_product_two_primes) {
    validate_on_big_product_two_primes(bpsw_fermat_prime_test_assume_prime, true, true);
}

TEST(bpsw_fermat_prime_test_stronger_lucas, small_squares) {
    for (BigInt x = 2; x < 300000; ++x) {
        auto status = bpsw_fermat_prime_test_assume_prime(x * x, true, true);
        EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(status);
    }
}

// proth_prime_test

TEST(proth_prime_test, small_proth) {
    for (uint64_t n = 1; n < 100; ++n) {
        for (uint64_t k = 1; k < 200 && (n >= 10 || k < (1 << n)); ++k) {
            // 2^n > k
            BigInt N = (BigInt(k) << n) + 1;
            Random rnd1(42);
            auto status_proth = proth_prime_test_assume_composite(N, 20, rnd1);
            Random rnd2(42);
            auto status_miller = miller_rabin_prime_test_assume_prime(N, 20, rnd2);
            EXPECT_TRUE(status_proth == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                    << to_string(status_proth);

        }
    }
}

TEST(proth_prime_test, big_proth_random) {
    Random rnd(42);
    for (size_t i = 0; i < 10; ++i) {
        auto n = static_cast<uint64_t>(rnd.uniform(1, 5000));
        auto k = rnd.uniform(1, (BigInt(1) << n) - 1);

        BigInt N = (k << n) + 1;
        Random rnd1(42);
        auto status_proth = proth_prime_test_assume_composite(N, 20, rnd1);
        Random rnd2(42);
        auto status_miller = miller_rabin_prime_test_assume_prime(N, 20, rnd2);
        EXPECT_TRUE(status_proth == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                << to_string(status_proth);
    }
}


TEST(proth_prime_test, big_proth_primes) {
    auto numbers = read_numbers(Filenames::BigProthPrimes);
    for (size_t i = 0; i < numbers.size(); i += 2) {
        auto n = static_cast<uint64_t>(numbers[i]);
        auto k = numbers[i + 1];
        auto N = (k << n) + 1;
        Random rnd(42);
        auto status = proth_prime_test_assume_composite(N, 20, rnd);
        EXPECT_TRUE(status == PrimalityStatus::Prime) << "Prime " << k << " * 2^" << n << " + 1 marked as "
                << to_string(status);
    }
}


}
