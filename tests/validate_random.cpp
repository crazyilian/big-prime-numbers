#include <gtest/gtest.h>
#include "common.h"
#include "random.hpp"

namespace BigPrimeLib {

// RandomSequence

void test_random_seq(int length, int seed) {
    DefaultRandomGenerator rnd(seed);

    RandomSequence rnd_seq(rnd, length);
    for (const auto &val : rnd_seq) {
        auto true_val = rnd();
        EXPECT_TRUE(val == true_val) << "RandomSequence(" << length << ", " << seed << ") value " << val
                << " is not equal to true value " << true_val << " during first run";
    }


    DefaultRandomGenerator rnd2(seed);
    for (const auto &val : rnd_seq) {
        auto true_val = rnd2();
        EXPECT_TRUE(val == true_val) << "RandomSequence(" << length << ", " << seed << ") value " << val
                << " is not equal to true value " << true_val << " during second run";
    }
}

TEST(RandomSequence, preserve_sequence_values) {
    test_random_seq(10, 42);
    test_random_seq(1000, 777);
}

// Random

TEST(Random, uniform) {
    Random rnd(42);
    int start = 7;
    int end = 16;
    int length = end - start + 1;
    int tests = 10000;

    std::map<int, int> counters;
    for (int i = 0; i < tests; ++i) {
        auto val = rnd.uniform(start, end);
        EXPECT_TRUE(start <= val && val <= end) << "Random.uniform(" << start << ", " << end << ") returned value "
                << val;
        counters[val.convert_to<int>()]++;
    }

    // counters[v] should be approx N(mu, sigma^2)
    // counters[v] is in [mu - 3*sigma , mu + 3*sigma] with probability of 99.7%
    auto p = 1.0 / length;
    auto sigma = sqrt(tests * p * (1 - p));
    auto mu = p * tests;
    auto min_cnt = mu - 3 * sigma;
    auto max_cnt = mu + 3 * sigma;

    for (int v = start; v <= end; ++v) {
        int cnt = 0;
        if (counters.contains(v)) {
            cnt = counters[v];
        }
        EXPECT_TRUE(min_cnt <= cnt && cnt <= max_cnt) << "Random.uniform(" << start << ", " << end
                << ") distribution is not uniform";
    }
}

}
