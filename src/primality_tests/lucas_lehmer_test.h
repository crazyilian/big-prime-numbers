#pragma once

#include "common.h"
#include "primality_utils.hpp"

namespace BigPrimeLib {

class LucasLehmerPrimeTester  {
public:
    LucasLehmerPrimeTester();
    PrimalityStatus test_raw(const BigInt &n);
    const PrimalityStatus &on_uncertain() const;

private:
    static constexpr PrimalityStatus on_uncertain_ = PrimalityStatus::Uncertain;
};

}
