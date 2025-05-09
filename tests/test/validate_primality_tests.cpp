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

namespace BigPrimeLib::Test {

namespace {

    void validate_on_small_primes(const std::unordered_set<BigInt> &skip, PrimeTesterRef prime_tester) {
        std::vector<BigInt> small_primes = read_numbers(Filenames::SmallPrimes);
        std::unordered_set<BigInt> small_primes_set(small_primes.begin(), small_primes.end());

        for (BigInt x = 2; x <= small_primes.back(); ++x) {
            if (skip.contains(x)) {
                continue;
            }
            bool is_prime = small_primes_set.contains(x);
            PrimalityStatus out = prime_tester->test(x);
            if (is_prime) {
                EXPECT_TRUE(out == PrimalityStatus::Prime)
                        << "Prime " << x << " marked as " << to_string(out);
            } else {
                EXPECT_TRUE(out == PrimalityStatus::Composite)
                        << "Composite " << x << " marked as " << to_string(out);
            }
        }
    }

    void validate_on_big_primes(PrimeTesterRef prime_tester) {
        std::vector<BigInt> big_primes = read_numbers(Filenames::BigPrimes);

        for (const BigInt &x : big_primes) {
            PrimalityStatus out = prime_tester->test(x);
            EXPECT_TRUE(out == PrimalityStatus::Prime) << "Prime " << x << " marked as " << to_string(out);
        }
    }

    void validate_on_big_product_two_primes(PrimeTesterRef prime_tester) {
        std::vector<BigInt> big_primes = read_numbers(Filenames::BigPrimes);

        for (size_t i = 0; i < big_primes.size(); ++i) {
            for (size_t j = i; j < big_primes.size(); ++j) {
                BigInt x = big_primes[i] * big_primes[j];
                PrimalityStatus out = prime_tester->test(x);
                EXPECT_TRUE(out == PrimalityStatus::Composite)
                    << "Composite " << big_primes[i] << "*" << big_primes[j] << " marked as " << to_string(out);
            }
        }
    }

    // trial_prime_test

    TEST(trial_prime_test, small_primes) {
        TrialPrimeTester t;
        validate_on_small_primes({}, t);
    }

    // fermat_prime_test

    TEST(fermat_prime_test, small_primes) {
        auto carmichael = read_numbers(Filenames::SmallCarmichael);
        std::unordered_set<BigInt> skip(carmichael.begin(), carmichael.end());
        FermatPrimeTester t(10, Random());
        validate_on_small_primes(skip, t);
    }

    TEST(fermat_prime_test, big_primes) {
        FermatPrimeTester t(10, Random());
        validate_on_big_primes(t);
    }

    TEST(fermat_prime_test, big_product_two_primes) {
        FermatPrimeTester t(10, Random());
        validate_on_big_product_two_primes(t);
    }

    // fermat_prime_test_iter

    TEST(fermat_prime_test_iter, small_primes) {
        std::vector<BigInt> bases(10);
        std::iota(bases.begin(), bases.end(), 2);
        auto carmichael = read_numbers(Filenames::SmallCarmichael);
        std::unordered_set<BigInt> skip(carmichael.begin(), carmichael.end());
        skip.insert(bases.begin(), bases.end());
        FermatPrimeTesterIter t(bases.begin(), bases.end());
        validate_on_small_primes(skip, t);
    }

    TEST(fermat_prime_test_iter, big_primes) {
        std::vector<BigInt> bases(10);
        std::iota(bases.begin(), bases.end(), 2);
        FermatPrimeTesterIter t(bases.begin(), bases.end());
        validate_on_big_primes(t);
    }

    TEST(fermat_prime_test_iter, big_product_two_primes) {
        std::vector<BigInt> bases(10);
        std::iota(bases.begin(), bases.end(), 2);
        FermatPrimeTesterIter t(bases.begin(), bases.end());
        validate_on_big_product_two_primes(t);
    }

    // miller_rabin_prime_test

    TEST(miller_rabin_prime_test, small_primes) {
        MillerRabinPrimeTester t(10, Random());
        validate_on_small_primes({}, t);
    }

    TEST(miller_rabin_prime_test, big_primes) {
        MillerRabinPrimeTester t(10, Random());
        validate_on_big_primes(t);
    }

    TEST(miller_rabin_prime_test, big_product_two_primes) {
        MillerRabinPrimeTester t(10, Random());
        validate_on_big_product_two_primes(t);
    }

    // miller_rabin_prime_test_iter

    TEST(miller_rabin_prime_test_iter, small_primes) {
        std::vector<BigInt> bases(10);
        std::iota(bases.begin(), bases.end(), 2);
        std::unordered_set<BigInt> skip(bases.begin(), bases.end());
        skip.insert(12);
        MillerRabinPrimeTesterIter t(bases.begin(), bases.end());;
        validate_on_small_primes(skip, t);
    }

    TEST(miller_rabin_prime_test_iter, big_primes) {
        std::vector<BigInt> bases(10);
        std::iota(bases.begin(), bases.end(), 2);
        MillerRabinPrimeTesterIter t(bases.begin(), bases.end());;
        validate_on_big_primes(t);
    }

    TEST(miller_rabin_prime_test_iter, big_product_two_primes) {
        std::vector<BigInt> bases(10);
        std::iota(bases.begin(), bases.end(), 2);
        MillerRabinPrimeTesterIter t(bases.begin(), bases.end());;
        validate_on_big_product_two_primes(t);
    }

    // lucas_lehmer_prime_test

    TEST(lucas_lehmer_prime_test, small_mersenne) {
        PrimeTester t = LucasLehmerPrimeTester();
        auto small_primes = read_numbers(Filenames::SmallPrimes);
        auto mersenne_primes = read_numbers(Filenames::MersennePrimes);
        std::set<BigInt> mersenne_primes_set(mersenne_primes.begin(), mersenne_primes.end());
        for (const BigInt &p : small_primes) {
            if (p > 3300) {
                break;
            }
            BigInt N = (BigInt(1) << static_cast<uint64_t>(p)) - 1;
            auto status = t.test(N);
            if (mersenne_primes_set.contains(p)) {
                EXPECT_TRUE(status == PrimalityStatus::Prime) << "Prime 2^" << p << "-1 marked as " <<
 to_string(status);
            } else {
                EXPECT_TRUE(status == PrimalityStatus::Composite) << "Prime 2^" << p << "-1 marked as "
                        << to_string(status);
            }
        }
    }

    // lucas_lehmer_riesel_prime_test

    TEST(lucas_lehmer_riesel_prime_test, small_generalized_mersenne) {
        PrimeTester t = LucasLehmerRieselPrimeTester();
        PrimeTester mrt = MillerRabinPrimeTester(20, Random());
        for (uint64_t n = 1; n < 100; ++n) {
            for (uint64_t k = 1; k < 200 && (n >= 10 || k < (1 << n)); ++k) {
                // 2^n > k
                BigInt N = (BigInt(k) << n) - 1;
                auto status_lucas = t.test(N);
                auto status_miller = mrt.test(N);
                EXPECT_TRUE(status_lucas == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                        << to_string(status_lucas);

            }
        }
    }

    TEST(lucas_lehmer_riesel_prime_test, big_generalized_mersenne_random) {
        PrimeTester t = LucasLehmerRieselPrimeTester();
        PrimeTester mrt = MillerRabinPrimeTester(20, Random());
        Random rnd(42);
        for (size_t i = 0; i < 10; ++i) {
            auto n = static_cast<uint64_t>(rnd.uniform(1, 5000));
            auto k = rnd.uniform(1, (BigInt(1) << n) - 1);

            BigInt N = (k << n) - 1;
            auto status_lucas = t.test(N);
            auto status_miller = mrt.test(N);
            EXPECT_TRUE(status_lucas == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                    << to_string(status_lucas);
        }
    }

    TEST(lucas_lehmer_riesel_prime_test, big_generalized_mersenne_primes) {
        PrimeTester t = LucasLehmerRieselPrimeTester();
        auto numbers = read_numbers(Filenames::BigGeneralizedMersennePrimes);
        for (size_t i = 0; i < numbers.size(); i += 2) {
            auto n = static_cast<uint64_t>(numbers[i]);
            auto k = numbers[i + 1];
            BigInt N = (k << n) - 1;
            auto status = t.test(N);
            EXPECT_TRUE(status == PrimalityStatus::Prime) << "Prime " << k << " * 2^" << n << " - 1 marked as "
                    << to_string(status);
        }
    }


    // bpsw_miller_prime_test

    TEST(bpsw_miller_prime_test, small_primes) {
        BPSWMillerPrimeTester t(true, false);
        validate_on_small_primes({}, t);
    }

    TEST(bpsw_miller_prime_test, big_primes) {
        BPSWMillerPrimeTester t(true, false);
        validate_on_big_primes(t);
    }

    TEST(bpsw_miller_prime_test, big_product_two_primes) {
        BPSWMillerPrimeTester t(true, false);
        validate_on_big_product_two_primes(t);
    }

    TEST(bpsw_miller_prime_test, small_squares) {
        PrimeTester t = BPSWMillerPrimeTester(true, false);
        for (BigInt x = 2; x < 300000; ++x) {
            auto status = t.test(x * x);
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(
                                                                 status);
        }
    }

    TEST(bpsw_miller_prime_test, wieferich_combinations) {
        PrimeTester t = BPSWMillerPrimeTester(true, false);
        BigInt w1 = 1093;
        BigInt w2 = 3511;
        for (size_t i = 0; i < 20; ++i) {
            for (size_t j = 0; j < 20; ++j) {
                if (i + j <= 1) {
                    continue;
                }
                BigInt x = Math::pow(w1, i) * Math::pow(w2, j);
                auto status = t.test(x);
                EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << " marked as "
                        << to_string(status);
            }
        }
    }

    // bpsw_miller_prime_test_no_wieferich

    TEST(bpsw_miller_prime_test_no_wieferich, small_primes) {
        BPSWMillerPrimeTester t(false, false);
        validate_on_small_primes({}, t);
    }

    TEST(bpsw_miller_prime_test_no_wieferich, big_primes) {
        BPSWMillerPrimeTester t(false, false);
        validate_on_big_primes(t);
    }

    TEST(bpsw_miller_prime_test_no_wieferich, big_product_two_primes) {
        BPSWMillerPrimeTester t(false, false);
        validate_on_big_product_two_primes(t);
    }

    TEST(bpsw_miller_prime_test_no_wieferich, small_squares) {
        PrimeTester t = BPSWMillerPrimeTester(false, false);
        for (BigInt x = 2; x < 300000; ++x) {
            auto status = t.test(x * x);
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(
                                                                 status);
        }
    }

    // bpsw_miller_prime_test_stronger_lucas

    TEST(bpsw_miller_prime_test_stronger_lucas, small_primes) {
        BPSWMillerPrimeTester t(true, true);
        validate_on_small_primes({}, t);
    }

    TEST(bpsw_miller_prime_test_stronger_lucas, big_primes) {
        BPSWMillerPrimeTester t(true, true);
        validate_on_big_primes(t);
    }

    TEST(bpsw_miller_prime_test_stronger_lucas, big_product_two_primes) {
        BPSWMillerPrimeTester t(true, true);
        validate_on_big_product_two_primes(t);
    }

    TEST(bpsw_miller_prime_test_stronger_lucas, small_squares) {
        PrimeTester t = BPSWMillerPrimeTester(true, true);
        for (BigInt x = 2; x < 300000; ++x) {
            auto status = t.test(x * x);
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(
                                                                 status);
        }
    }

    // bpsw_fermat_prime_test

    TEST(bpsw_fermat_prime_test, small_primes) {
        BPSWFermatPrimeTester t(true, false);
        validate_on_small_primes({}, t);
    }

    TEST(bpsw_fermat_prime_test, big_primes) {
        BPSWFermatPrimeTester t(true, false);
        validate_on_big_primes(t);
    }

    TEST(bpsw_fermat_prime_test, big_product_two_primes) {
        BPSWFermatPrimeTester t(true, false);
        validate_on_big_product_two_primes(t);
    }

    TEST(bpsw_fermat_prime_test, small_squares) {
        PrimeTester t = BPSWFermatPrimeTester(true, false);
        for (BigInt x = 2; x < 300000; ++x) {
            auto status = t.test(x * x);
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(
                                                                 status);
        }
    }

    TEST(bpsw_fermat_prime_test, wieferich_combinations) {
        PrimeTester t = BPSWFermatPrimeTester(true, false);
        BigInt w1 = 1093;
        BigInt w2 = 3511;
        for (size_t i = 0; i < 20; ++i) {
            for (size_t j = 0; j < 20; ++j) {
                if (i + j <= 1) {
                    continue;
                }
                BigInt x = Math::pow(w1, i) * Math::pow(w2, j);
                auto status = t.test(x);
                EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << " marked as "
                        << to_string(status);
            }
        }
    }

    // bpsw_fermat_prime_test_no_wieferich

    TEST(bpsw_fermat_prime_test_no_wieferich, small_primes) {
        BPSWFermatPrimeTester t(false, false);
        validate_on_small_primes({}, t);
    }

    TEST(bpsw_fermat_prime_test_no_wieferich, big_primes) {
        BPSWFermatPrimeTester t(false, false);
        validate_on_big_primes(t);
    }

    TEST(bpsw_fermat_prime_test_no_wieferich, big_product_two_primes) {
        BPSWFermatPrimeTester t(false, false);
        validate_on_big_product_two_primes(t);
    }

    TEST(bpsw_fermat_prime_test_no_wieferich, small_squares) {
        PrimeTester t = BPSWFermatPrimeTester(false, false);
        for (BigInt x = 2; x < 300000; ++x) {
            auto status = t.test(x * x);
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(
                                                                 status);
        }
    }

    // bpsw_fermat_prime_test_stronger_lucas

    TEST(bpsw_fermat_prime_test_stronger_lucas, small_primes) {
        BPSWFermatPrimeTester t(true, true);
        validate_on_small_primes({}, t);
    }

    TEST(bpsw_fermat_prime_test_stronger_lucas, big_primes) {
        BPSWFermatPrimeTester t(true, true);
        validate_on_big_primes(t);
    }

    TEST(bpsw_fermat_prime_test_stronger_lucas, big_product_two_primes) {
        BPSWFermatPrimeTester t(true, true);
        validate_on_big_product_two_primes(t);
    }

    TEST(bpsw_fermat_prime_test_stronger_lucas, small_squares) {
        PrimeTester t = BPSWFermatPrimeTester(true, true);
        for (BigInt x = 2; x < 300000; ++x) {
            auto status = t.test(x * x);
            EXPECT_TRUE(status == PrimalityStatus::Composite) << "Composite " << x << "^2 marked as " << to_string(
                                                                 status);
        }
    }

    // proth_prime_test

    TEST(proth_prime_test, small_proth) {
        PrimeTester t = ProthPrimeTester(20, Random());
        PrimeTester mrt = MillerRabinPrimeTester(20, Random());
        for (uint64_t n = 1; n < 100; ++n) {
            for (uint64_t k = 1; k < 200 && (n >= 10 || k < (1 << n)); ++k) {
                // 2^n > k
                BigInt N = (BigInt(k) << n) + 1;
                auto status_proth = t.test(N);
                auto status_miller = mrt.test(N);
                EXPECT_TRUE(status_proth == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                        << to_string(status_proth);

            }
        }
    }

    TEST(proth_prime_test, big_proth_random) {
        PrimeTester t = ProthPrimeTester(20, Random());
        PrimeTester mrt = MillerRabinPrimeTester(20, Random());
        Random rnd(42);
        for (size_t i = 0; i < 10; ++i) {
            auto n = static_cast<uint64_t>(rnd.uniform(1, 5000));
            auto k = rnd.uniform(1, (BigInt(1) << n) - 1);

            BigInt N = (k << n) + 1;
            auto status_proth = t.test(N);
            auto status_miller = mrt.test(N);
            EXPECT_TRUE(status_proth == status_miller) << to_string(status_miller) << ' ' << N << " marked as "
                    << to_string(status_proth);
        }
    }

}

}
