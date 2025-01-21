enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(
        COPY
            ${CMAKE_CURRENT_SOURCE_DIR}/first_10000_primes.txt
            ${CMAKE_CURRENT_SOURCE_DIR}/random_big_primes.txt
        DESTINATION
            ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(
        tests
        validate_primality_tests.cpp
        small_validator.hpp
        big_validator.hpp
        test_utils.cpp
        test_utils.h
)
target_link_libraries(tests big_prime_lib GTest::gtest_main)

gtest_discover_tests(tests)
