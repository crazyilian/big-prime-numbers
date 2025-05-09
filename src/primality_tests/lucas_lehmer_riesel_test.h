#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

class LucasLehmerRieselPrimeTester {
public:
    LucasLehmerRieselPrimeTester();
    const PrimalityStatus &on_uncertain() const;
    PrimalityStatus test_raw(const BigInt &n);

private:
    static constexpr PrimalityStatus on_uncertain_ = PrimalityStatus::Uncertain;
};

}
