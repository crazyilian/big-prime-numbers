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


namespace BigPrimeLib::Test {

namespace {

    void register_benchmark(std::string name, const BigInt &n, PrimeTester &&t) {
        benchmark::RegisterBenchmark(
            name,
            [](benchmark::State &state, const BigInt &n, PrimeTester t) {
                for (auto _ : state) {
                    benchmark::DoNotOptimize(t.test(n));
                }
            },
            n, std::move(t)
        );
    }

    void register_benchmarks_mersenne_prime() {
        std::vector<int> exponents = {3217, 4253, 4423, 9689, 9941, 11213, 19937};
        std::string data_name = "mersenne_prime";
        for (int e : exponents) {
            BigInt n = (BigInt(1) << e) - 1;
            register_benchmark(data_name + "/lucas_lehmer/" + std::to_string(e), n,
                               LucasLehmerPrimeTester());
            register_benchmark(data_name + "/lucas_lehmer_riesel/" + std::to_string(e), n,
                               LucasLehmerRieselPrimeTester());
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
        }
    }

    void register_benchmarks_mersenne_composite() {
        std::vector<int> exponents = {4000, 6000, 8000, 10000, 12000, 14000, 16000, 18000, 20000};
        std::string data_name = "mersenne_composite";
        for (int e : exponents) {
            BigInt n = (BigInt(1) << e) - 1;
            register_benchmark(data_name + "/lucas_lehmer/" + std::to_string(e), n,
                               LucasLehmerPrimeTester());
            register_benchmark(data_name + "/lucas_lehmer_riesel/" + std::to_string(e), n,
                               LucasLehmerRieselPrimeTester());
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
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
            register_benchmark(data_name + "/lucas_lehmer_riesel/" + std::to_string(e), n,
                               LucasLehmerRieselPrimeTester());
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
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
            register_benchmark(data_name + "/proth_k10/" + std::to_string(e), n,
                               ProthPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/proth_k20/" + std::to_string(e), n,
                               ProthPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
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
            register_benchmark(data_name + "/proth_k10/" + std::to_string(e), n,
                               ProthPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/proth_k20/" + std::to_string(e), n,
                               ProthPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
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
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
        }
    }

    void register_benchmarks_random_prime() {
        auto tests = read_numbers(Filenames::VeryBigPrimes);
        std::string data_name = "random_prime";
        for (const auto &n : tests) {
            size_t e = Math::msb(n);
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
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
            register_benchmark(data_name + "/bpsw_fermat/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_fermat_stronger/" + std::to_string(e), n,
                               BPSWFermatPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/bpsw_miller_rabin/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich));
            register_benchmark(data_name + "/bpsw_miller_rabin_stronger/" + std::to_string(e), n,
                               BPSWMillerPrimeTester(BPSWFeatures::KnownWieferich | BPSWFeatures::StrongerLucas));
            register_benchmark(data_name + "/miller_rabin_k10/" + std::to_string(e), n,
                               MillerRabinPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/miller_rabin_k20/" + std::to_string(e), n,
                               MillerRabinPrimeTester(20, Random(), true));
            register_benchmark(data_name + "/fermat_k10/" + std::to_string(e), n,
                               FermatPrimeTester(10, Random(), true));
            register_benchmark(data_name + "/fermat_k20/" + std::to_string(e), n,
                               FermatPrimeTester(20, Random(), true));
        }
    }

    void register_all_benchmarks() {
        register_benchmarks_mersenne_prime();
        register_benchmarks_mersenne_composite();
        register_benchmarks_generalized_mersenne_prime();
        register_benchmarks_proth_prime();
        register_benchmarks_proth_composite();
        register_benchmarks_semiprime();
        register_benchmarks_random_prime();
        register_benchmarks_random_composite();
    }

}

}

int main(int argc, char **argv) {
    BigPrimeLib::Test::register_all_benchmarks();
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) {
        return 1;
    }
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
}
