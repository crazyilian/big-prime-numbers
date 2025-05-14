#pragma once

#include "prime_tester_interface.hpp"

namespace BigPrimeLib {

using PrimeTester = folly::Poly<detail::IPrimeTester>;
using PrimeTesterRef = folly::Poly<detail::IPrimeTester &>;

}
