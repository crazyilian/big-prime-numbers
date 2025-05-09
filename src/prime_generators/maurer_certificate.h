#pragma once
#include <list>

#include "common.h"

namespace BigPrimeLib {

bool verify_certificate_step(const BigInt &p, const BigInt &q, const BigInt &r, const BigInt &a);

class MaurerPrime {
public:
    struct CertificateStepInfo {
        BigInt r, a;
    };

    struct CertificateStep {
        BigInt p;
        std::optional<CertificateStepInfo> info;
    };

    explicit MaurerPrime(const BigInt &p);

    MaurerPrime &extend_prime(const BigInt &p, const BigInt &r, const BigInt &a);
    MaurerPrime &truncate_prime();
    bool verify_assuming_prime_base() const;
    const BigInt &p() const;
    const std::list<CertificateStep> &certificate() const;

private:
    std::list<CertificateStep> cert_;
};


}
