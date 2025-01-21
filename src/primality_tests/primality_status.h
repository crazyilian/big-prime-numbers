#pragma once

namespace BigPrimeLib {

enum class PrimalityStatus {
    Prime,
    Composite,
    Uncertain,
    NotApplicable
};

const char *to_string(PrimalityStatus s);

}
