#pragma once

#include "common.h"
#include "primality_utils.hpp"

namespace BigPrimeLib {

class LucasLehmerPrimeTester : public PrimeTester {
public:
    LucasLehmerPrimeTester();
    PrimalityStatus test_raw(const BigInt &n) override;
};

}
