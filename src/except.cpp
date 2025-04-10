#include "except.h"
#include <format>
#include <iostream>

namespace BigPrimeLib {

namespace Except {

    CantOpenFileException::CantOpenFileException(const char *filename)
        : std::runtime_error(std::format("Can't open file \"{}\".", filename)) {}

    OutOfRangeException::OutOfRangeException(const char *what)
        : std::out_of_range(std::format("{} out of range.", what)) {}

    void react() {
        try {
            throw;
        } catch (std::exception & &e) {
            std::cerr << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error!" << std::endl;
        }
    }

}

}
