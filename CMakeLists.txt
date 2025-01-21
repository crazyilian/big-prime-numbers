cmake_minimum_required(VERSION 3.29)
project(big_prime_numbers)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,bounds,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

find_package(Boost REQUIRED)

include_directories(src)
add_library(
        big_prime_lib
        src/primality_tests/primality_status.cpp
        src/primality_tests/trial_test.cpp
        src/primality_tests/fermat_test.hpp
        tests/validate_primality_tests.cpp
)

target_include_directories(big_prime_lib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(big_prime_lib gmp)

add_subdirectory(example)
add_subdirectory(tests)
