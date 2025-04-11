#include "primality_utils.h"

namespace BigPrimeLib {

const char *to_string(PrimalityStatus s) {
    switch (s) {
        case PrimalityStatus::Prime: return "Prime";
        case PrimalityStatus::Composite: return "Composite";
        case PrimalityStatus::Uncertain: return "Uncertain";
        case PrimalityStatus::NotApplicable: return "NotApplicable";
        default: return "[Unknown PrimalityStatus]";
    }
}

PrimalityStatus test_leq_3(const BigInt &n) {
    if (n <= 1) {
        return PrimalityStatus::NotApplicable;
    }
    if (n <= 3) {
        return PrimalityStatus::Prime;
    }
    return PrimalityStatus::Uncertain;
}

PrimalityStatus uncertain2prime(PrimalityStatus s) {
    return s == PrimalityStatus::Uncertain ? PrimalityStatus::Prime : s;
}

}
