#pragma once

#include <boost/random.hpp>
#include "common.h"

namespace BigPrimeLib {

using DefaultRandomGenerator = boost::random::mt19937;

template<class Generator = DefaultRandomGenerator>
class Random {
    using Uniform = boost::random::uniform_int_distribution<BigInt>;

public:
    explicit Random(uint32_t seed = kDefaultSeed) : generator_(seed) {}

    BigInt uniform(BigInt a, BigInt b) {
        Uniform dist(a, b);
        return dist(generator_);
    }

private:
    Generator generator_;
    static constexpr uint32_t kDefaultSeed = 42;
};

template<class DistributionGenerator>
class RandomSequence {
public:
    RandomSequence(DistributionGenerator random, size_t length)
        : random_(random), length_(length) {}

    class Iterator {
    public:
        Iterator(DistributionGenerator random, size_t remaining)
            : random_(random), remaining_(remaining), current_(0) {
            if (remaining_ != 0) {
                generate_new_value();
            }
        }

        const BigInt &operator*() const {
            assert(remaining_ != 0 && "RandomSequence iterator is out of range.");
            return current_;
        }

        Iterator &operator++() {
            assert(remaining_ != 0 && "RandomSequence iterator is out of range.");
            generate_new_value();
            --remaining_;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        bool operator==(const Iterator &other) const {
            return remaining_ == other.remaining_;
        }

        bool operator!=(const Iterator &other) const {
            return !(*this == other);
        }

    private:
        void generate_new_value() {
            current_ = random_();
        }

    private:
        DistributionGenerator random_;
        size_t remaining_;
        BigInt current_;
    };

    Iterator begin() const {
        return Iterator(random_, length_);
    }

    Iterator end() const {
        return Iterator(random_, 0);
    }

private:
    DistributionGenerator random_;
    size_t length_;
};

}
