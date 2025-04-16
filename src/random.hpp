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

    template<class SingleArg, class = std::enable_if_t<!std::is_same_v<Random, SingleArg>>>
    explicit Random(const SingleArg &arg) : generator_(arg) {}

    template<class... Args, class = std::enable_if_t<(sizeof...(Args) != 1)>>
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
