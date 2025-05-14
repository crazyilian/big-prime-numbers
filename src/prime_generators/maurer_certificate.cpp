#include <list>

#include "maurer_certificate.h"

namespace BigPrimeLib {

bool verify_certificate_step(const BigInt &p, const BigInt &q, const BigInt &r, const BigInt &a) {
    if (p != 2 * r * q + 1) {
        return false;
    }
    auto b = Math::powm(a, 2 * r, p);
    if (b == 1 || Math::powm(b, q, p) != 1) {
        return false;
    }
    return true;
}

MaurerPrime::MaurerPrime(const BigInt &p) : cert_({{p, std::nullopt}}) {}

MaurerPrime &MaurerPrime::extend_prime(const BigInt &p, const BigInt &r, const BigInt &a) {
    assert(verify_certificate_step(p, this->p(), r, a));
    cert_.emplace_front(p, CertificateStepInfo{r, a});
    return *this;
}

MaurerPrime &MaurerPrime::truncate_prime() {
    assert(!cert_.empty());
    cert_.pop_front();
    return *this;
}

bool MaurerPrime::verify_assuming_prime_base() const {
    size_t verified_steps = 0;
    if (cert_.back().info.has_value()) {
        return false;
    }
    for (auto it = cert_.cbegin(); it != cert_.cend(); ++verified_steps) {
        if (!it->info.has_value()) {
            break;
        }
        const auto &p = it->p;
        const auto &[r, a] = it->info.value();
        const auto &q = (++it)->p;
        if (!verify_certificate_step(p, q, r, a)) {
            return false;
        }
    }
    if (verified_steps + 1 != cert_.size()) {
        return false;
    }
    return true;
}

const BigInt &MaurerPrime::p() const { return cert_.front().p; }
const std::list<MaurerPrime::CertificateStep> &MaurerPrime::certificate() const { return cert_; }

}
