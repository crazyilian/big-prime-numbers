#pragma once

#include <boost/random.hpp>
#include "common.h"
#include "except.h"

namespace BigPrimeLib {

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

template<class DistributionGenerator>
class RandomSequence {
public:
    RandomSequence(const DistributionGenerator &random, size_t length)
        : random_(random), length_(length) {}

    class Iterator {
    public:
        Iterator(const DistributionGenerator &random, size_t remaining)
            : random_(random), remaining_(remaining), current_(0) {
            if (remaining_ != 0) {
                next();
            }
        }

        BigInt operator*() const {
            if (remaining_ == 0) {
                throw Except::OutOfRangeException("RandomSequence iterator");
            }
            return current_;
        }

        Iterator &operator++() {
            if (remaining_ == 0) {
                throw Except::OutOfRangeException("RandomSequence iterator");
            }
            next();
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
        const DistributionGenerator &random_;
        size_t remaining_;
        BigInt current_;

        void next() {
            current_ = random_();
        }
    };

    Iterator begin() {
        return Iterator(random_, length_);
    }

    Iterator end() {
        return Iterator(random_, 0);
    }

private:
    const DistributionGenerator &random_;
    size_t length_;
};


}
