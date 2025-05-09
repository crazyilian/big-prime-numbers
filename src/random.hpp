#pragma once

#include <boost/random.hpp>
#include "common.h"

namespace BigPrimeLib {

using DefaultRandomGenerator = boost::random::mt19937;

template<class Generator = DefaultRandomGenerator>
class Random {
    using Uniform = boost::random::uniform_int_distribution<BigInt>;
    using UniformReal = boost::random::uniform_real_distribution<double>;

public:
    Random(const Random &) = default;
    Random(Random &) = default;
    Random(Random &&) noexcept = default;
    Random(const Random &&) noexcept = delete;
    Random &operator=(const Random &) = default;
    Random &operator=(Random &&) noexcept = default;

    template<class... Args>
    explicit Random(Args &&... args) : generator_(std::forward<Args>(args)...) {}

    BigInt uniform(BigInt a, BigInt b) {
        Uniform dist(a, b);
        return dist(generator_);
    }

    double uniform_real(double a, double b) {
        UniformReal dist(a, b);
        return dist(generator_);
    }

private:
    Generator generator_;
};

}
