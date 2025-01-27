#include <exception>
#include "except.h"

#include <format>
#include <iostream>
#include <stdexcept>

namespace BigPrimeLib {

namespace Except {

    CantOpenFileException::CantOpenFileException(const char *filename)
        : std::runtime_error(std::format("Can't open file \"{}\".", filename)) {}

    void react() {
        try {
            throw;
        } catch (CantOpenFileException &e) {
            std::cerr << e.what() << std::endl;
        } catch (...) {
            std::cerr << "Unknown error!" << std::endl;
        }
    }

}

}
