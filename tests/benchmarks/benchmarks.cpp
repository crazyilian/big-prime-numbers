#include <benchmark/benchmark.h>
#include "common.h"
#include "primality_tests/bpsw_test.cpp"
#include "primality_tests/trial_test.h"
#include "primality_tests/fermat_test.hpp"
#include "primality_tests/miller_rabin_test.hpp"
#include "primality_tests/lucas_lehmer_test.h"
#include "primality_tests/lucas_lehmer_riesel_test.h"
#include "primality_tests/proth_test.hpp"
#include "test_utils.h"


namespace BigPrimeLib {

void run_test(benchmark::State &state, PrimeTesterRef prime_tester, const BigInt &n) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(prime_tester->test(n));
    }
}


template<class Tester, class... Args>
void register_benchmark(std::string name, const BigInt &n, Args &&... args) {
    benchmark::RegisterBenchmark(
        name,
        [args...](benchmark::State &state, const BigInt &n) {
            Tester t(args...);
            run_test(state, t, n);
        },
        n
    );
}


void register_benchmarks_mersenne_prime() {
    std::vector<int> exponents = {3217, 4253, 4423, 9689, 9941, 11213, 19937};
    std::string data_name = "mersenne_prime";
    for (int e : exponents) {
        BigInt n = (BigInt(1) << e) - 1;
        register_benchmark<LucasLehmerPrimeTester>(data_name + "/lucas_lehmer/" + std::to_string(e), n);
        register_benchmark<LucasLehmerRieselPrimeTester>(data_name + "/lucas_lehmer_riesel/" + std::to_string(e), n);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}


void register_benchmarks_mersenne_composite() {
    std::vector<int> exponents = {4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000};
    std::string data_name = "mersenne_composite";
    for (int e : exponents) {
        BigInt n = (BigInt(1) << e) - 1;
        register_benchmark<LucasLehmerPrimeTester>(data_name + "/lucas_lehmer/" + std::to_string(e), n);
        register_benchmark<LucasLehmerRieselPrimeTester>(data_name + "/lucas_lehmer_riesel/" + std::to_string(e), n);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}

void register_benchmarks_generalized_mersenne_prime() {
    auto numbers = read_numbers(Filenames::BigGeneralizedMersennePrimes);
    std::vector<BigInt> tests;
    for (size_t i = 0; i < numbers.size(); i += 2) {
        auto n = static_cast<uint64_t>(numbers[i]);
        auto k = numbers[i + 1];
        tests.push_back((k << n) - 1);
    }
    std::string data_name = "k2n-1_prime";

    for (const auto &n : tests) {
        size_t e = Math::msb(n);
        if (e <= 3000) {
            continue;
        }
        register_benchmark<LucasLehmerRieselPrimeTester>(data_name + "/lucas_lehmer_riesel/" + std::to_string(e), n);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}

void register_benchmarks_proth_prime() {
    auto numbers = read_numbers(Filenames::BigProthPrimes);
    std::vector<BigInt> tests;
    for (size_t i = 0; i < numbers.size(); i += 2) {
        auto n = static_cast<uint64_t>(numbers[i]);
        auto k = numbers[i + 1];
        tests.push_back((k << n) + 1);
    }
    std::string data_name = "k2n+1_prime";

    for (const auto &n : tests) {
        size_t e = Math::msb(n);
        if (e <= 3000) {
            continue;
        }
        register_benchmark<ProthPrimeTester<DefaultRandomGenerator>>(
            data_name + "/proth_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<ProthPrimeTester<DefaultRandomGenerator>>(
            data_name + "/proth_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}

void register_benchmarks_proth_composite() {
    std::vector<BigInt> tests;
    Random rnd;
    PrimeTester mrt = MillerRabinPrimeTester(40, Random(), true);
    for (size_t i = 4; i <= 10; ++i) {
        size_t n = i * 1000 / 2;
        BigInt N;
        do {
            auto k = rnd.uniform(1, (BigInt(1) << n) - 1);
            if (k % 2 == 0) {
                k++;
            }
            N = (k << n) + 1;
        } while (mrt.test(N) != PrimalityStatus::Composite);
        tests.push_back(N);
    }
    std::string data_name = "k2n+1_composite";

    for (const auto &n : tests) {
        size_t e = Math::msb(n);
        register_benchmark<ProthPrimeTester<DefaultRandomGenerator>>(
            data_name + "/proth_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<ProthPrimeTester<DefaultRandomGenerator>>(
            data_name + "/proth_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}

void register_benchmarks_semiprime() {
    auto numbers = read_numbers(Filenames::BigSemiprimes);
    std::vector<BigInt> tests;
    for (size_t i = 0; i < numbers.size(); i += 2) {
        auto a = numbers[i];
        auto b = numbers[i + 1];
        tests.push_back(a * b);
    }
    std::string data_name = "semiprime";
    for (const auto &n : tests) {
        size_t e = Math::msb(n);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}

void register_benchmarks_random_prime() {
    auto tests = read_numbers(Filenames::VeryBigPrimes);
    std::string data_name = "random_prime";
    for (const auto &n : tests) {
        size_t e = Math::msb(n);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}

void register_benchmarks_random_composite() {
    std::vector<BigInt> tests;
    Random rnd;
    PrimeTester mrt = MillerRabinPrimeTester(40, Random(), true);
    for (size_t i = 4; i <= 10; ++i) {
        size_t n = i * 1000;
        BigInt N;
        do {
            N = rnd.uniform((BigInt(1) << n), (BigInt(1) << (n + 1)));
            if (N % 2 == 0) {
                N++;
            }
        } while (mrt.test(N) != PrimalityStatus::Composite);
        tests.push_back(N);
    }
    std::string data_name = "random_composite";

    for (const auto &n : tests) {
        size_t e = Math::msb(n);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWFermatPrimeTester>(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                                                  true, false, true);
        register_benchmark<BPSWMillerPrimeTester>(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                                                  true, true, true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<MillerRabinPrimeTester<DefaultRandomGenerator>>(
            data_name + "/miller_rabin_k20/" + std::to_string(e), n, 20, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k10/" + std::to_string(e), n, 10, Random(), true);
        register_benchmark<FermatPrimeTester<DefaultRandomGenerator>>(
            data_name + "/fermat_k20/" + std::to_string(e), n, 20, Random(), true);
    }
}


auto _ = []() {
    register_benchmarks_mersenne_prime();
    register_benchmarks_mersenne_composite();
    register_benchmarks_generalized_mersenne_prime();
    register_benchmarks_proth_prime();
    register_benchmarks_proth_composite();
    register_benchmarks_semiprime();
    register_benchmarks_random_prime();
    register_benchmarks_random_composite();
    return 0;
}();


BENCHMARK_MAIN();
}
