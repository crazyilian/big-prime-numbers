#pragma once
#include <stdexcept>

namespace BigPrimeLib {

namespace Except {

    class CantOpenFileException : public std::runtime_error {
    public:
        explicit CantOpenFileException(const char *filename);
    };

    void react();

}

}
