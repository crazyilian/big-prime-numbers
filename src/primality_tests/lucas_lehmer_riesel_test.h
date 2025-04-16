#pragma once

#include "common.h"
#include "primality_utils.hpp"

namespace BigPrimeLib {

class LucasLehmerRieselPrimeTester : public PrimeTester {
public:
    LucasLehmerRieselPrimeTester();
    PrimalityStatus test_raw(const BigInt &n) override;
};

}
