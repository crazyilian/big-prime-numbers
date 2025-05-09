#pragma once

#include "common.h"
#include "factorizer_interface.hpp"

namespace BigPrimeLib {

using Factorizer = folly::Poly<detail::IFactorizer>;
using FactorizerRef = folly::Poly<detail::IFactorizer &>;

}
