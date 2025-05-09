#pragma once

#include "common.h"
#include "primality_utils.hpp"

namespace BigPrimeLib {

namespace detail {

    class BPSWPrimeTester {
    public:
        explicit BPSWPrimeTester(bool known_wieferich = true, bool stronger_lucas = false, bool assume_prime = true);
        const PrimalityStatus &on_uncertain() const;

    protected:
        PrimalityStatus lucas_test_raw(const BigInt &n) const;

    private:
        PrimalityStatus on_uncertain_;
        bool known_wieferich_, stronger_lucas_;
    };

}

class BPSWMillerPrimeTester : public detail::BPSWPrimeTester {
public:
    using BPSWPrimeTester::BPSWPrimeTester;
    PrimalityStatus test_raw(const BigInt &n);
};

class BPSWFermatPrimeTester : public detail::BPSWPrimeTester {
public:
    using BPSWPrimeTester::BPSWPrimeTester;
    PrimalityStatus test_raw(const BigInt &n);
};

}
