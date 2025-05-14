#pragma once
#include <stdexcept>

namespace BigPrimeLib {

namespace Except {

    class CantOpenFileException : public std::runtime_error {
    public:
        explicit CantOpenFileException(const char *filename);
    };

    class OutOfRangeException : public std::out_of_range {
    public:
        explicit OutOfRangeException(const char *what);
    };

    void react();

}

}
