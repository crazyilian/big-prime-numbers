cmake_minimum_required(VERSION 3.29)
project(big_prime_lib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED)

include_directories(src)
add_library(
        big_prime_lib
        src/primality_tests/primality_utils.cpp
        src/primality_tests/trial_test.cpp
        src/except.cpp
)

target_compile_options(big_prime_lib PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_compile_options(big_prime_lib PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,bounds,undefined>)
target_link_options(big_prime_lib PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,bounds,undefined>)
target_link_libraries(big_prime_lib PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,bounds,undefined>)
target_compile_options(big_prime_lib PRIVATE $<$<CONFIG:Release>:-O3>)

target_include_directories(big_prime_lib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(big_prime_lib PRIVATE gmp)

add_subdirectory(example)
add_subdirectory(tests)
