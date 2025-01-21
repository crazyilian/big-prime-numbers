#pragma once

#include <boost/multiprecision/gmp.hpp>
#include <boost/random.hpp>

namespace BigPrimeLib {

using BigInt = boost::multiprecision::mpz_int;
using UniformBigIntDistribution = boost::random::uniform_int_distribution<BigInt>;

}
