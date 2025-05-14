#pragma once

#include "common.h"
#include "primality_utils.h"

namespace BigPrimeLib {

enum class BPSWFeatures {
    None = 0,
    KnownWieferich = 1,
    StrongerLucas = 2
};

BPSWFeatures operator&(BPSWFeatures, BPSWFeatures);
BPSWFeatures &operator&=(BPSWFeatures &, BPSWFeatures);
BPSWFeatures operator|(BPSWFeatures, BPSWFeatures);
BPSWFeatures &operator|=(BPSWFeatures &, BPSWFeatures);

namespace detail {

    class BPSWPrimeTester {
    public:
        explicit BPSWPrimeTester(BPSWFeatures features = BPSWFeatures::KnownWieferich, bool assume_prime = true);
        const PrimalityStatus &on_uncertain() const;

    protected:
        PrimalityStatus lucas_test_raw(const BigInt &n) const;

    private:
        PrimalityStatus on_uncertain_;
        BPSWFeatures features_;
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
