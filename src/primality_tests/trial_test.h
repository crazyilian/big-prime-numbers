#pragma once

#include "common.h"
#include "primality_utils.hpp"

namespace BigPrimeLib {

class TrialPrimeTester : public PrimeTester {
public:
    TrialPrimeTester();
    PrimalityStatus test_raw(const BigInt &n) override;
    std::unique_ptr<PrimeTester> clone() const override;
};


}
