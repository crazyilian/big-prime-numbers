#pragma once

#include "common.h"
#include "primality_utils.hpp"

namespace BigPrimeLib {

class LucasLehmerPrimeTester {
public:
    LucasLehmerPrimeTester();
    const PrimalityStatus &on_uncertain() const;
    PrimalityStatus test_raw(const BigInt &n);

private:
    static constexpr PrimalityStatus on_uncertain_ = PrimalityStatus::Uncertain;
};

}
