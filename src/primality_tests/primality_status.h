#pragma once

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

}
