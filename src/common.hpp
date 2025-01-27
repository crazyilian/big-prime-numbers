#pragma once

#include <boost/multiprecision/gmp.hpp>
#include <boost/random.hpp>

namespace BigPrimeLib {

using BigInt = boost::multiprecision::mpz_int;
using DefaultRandomGenerator = boost::random::mt19937;

template<class Generator = DefaultRandomGenerator>
class Random {
    using Uniform = boost::random::uniform_int_distribution<BigInt>;

public:
    explicit Random(unsigned seed = DEFAULT_SEED) : generator_(seed) {}

    BigInt uniform(BigInt a, BigInt b) {
        Uniform dist(a, b);
        return dist(generator_);
    }

private:
    Generator generator_;
    static constexpr unsigned DEFAULT_SEED = 42;
};

}
