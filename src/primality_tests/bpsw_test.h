#pragma once

#include "common.h"
#include "primality_utils.hpp"

namespace BigPrimeLib {

class BPSWPrimeTester : public PrimeTester {
public:
    bool known_wieferich, stronger_lucas;

public:
    explicit BPSWPrimeTester(bool known_wieferich = true, bool stronger_lucas = false, bool assume_prime = true);

protected:
    PrimalityStatus lucas_test_wrapper(const BigInt &n) const;
};

class BPSWMillerPrimeTester : public BPSWPrimeTester {
public:
    using BPSWPrimeTester::BPSWPrimeTester;
    PrimalityStatus test_raw(const BigInt &n) override;
    std::unique_ptr<PrimeTester> clone() const override;
};

class BPSWFermatPrimeTester : public BPSWPrimeTester {
public:
    using BPSWPrimeTester::BPSWPrimeTester;
    PrimalityStatus test_raw(const BigInt &n) override;
    std::unique_ptr<PrimeTester> clone() const override;
};

}
