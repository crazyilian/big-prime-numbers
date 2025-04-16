#pragma once

#include "common.h"

namespace BigPrimeLib {

enum class PrimalityStatus {
    Prime,
    Composite,
    Uncertain,
    NotApplicable
};

inline const char *to_string(PrimalityStatus s) {
    switch (s) {
        case PrimalityStatus::Prime: return "Prime";
        case PrimalityStatus::Composite: return "Composite";
        case PrimalityStatus::Uncertain: return "Uncertain";
        case PrimalityStatus::NotApplicable: return "NotApplicable";
        default: return "[Unknown PrimalityStatus]";
    }
}

inline PrimalityStatus test_leq_3(const BigInt &n) {
    if (n <= 1) {
        return PrimalityStatus::NotApplicable;
    }
    if (n <= 3) {
        return PrimalityStatus::Prime;
    }
    return PrimalityStatus::Uncertain;
}

class PrimeTester {
public:
    PrimalityStatus on_uncertain;

public:
    explicit PrimeTester(PrimalityStatus on_uncertain) : on_uncertain(on_uncertain) {}

    virtual ~PrimeTester() = default;
    virtual PrimalityStatus test_raw(const BigInt &n) = 0;

    PrimalityStatus test(const BigInt &n) {
        auto status = test_raw(n);
        return status == PrimalityStatus::Uncertain ? on_uncertain : status;
    }
};

template<class Iterator>
class PrimeTesterIter : public PrimeTester {
public:
    std::optional<Iterator> default_begin, default_end;

public:
    explicit PrimeTesterIter(PrimalityStatus on_uncertain)
        : PrimeTester(on_uncertain), default_begin(std::nullopt), default_end(std::nullopt) {}
    PrimeTesterIter(Iterator default_begin, Iterator default_end, PrimalityStatus on_uncertain)
        : PrimeTester(on_uncertain), default_begin(default_begin), default_end(default_end) {}

public:
    virtual PrimalityStatus test_raw(const BigInt &n, Iterator begin, Iterator end) = 0;

    PrimalityStatus test_raw(const BigInt &n) override {
        return test_raw(n, default_begin.value(), default_end.value());
    }

    PrimalityStatus test(const BigInt &n, Iterator begin, Iterator end) {
        auto status = test_raw(n, begin, end);
        return status == PrimalityStatus::Uncertain ? on_uncertain : status;
    }

};

}
