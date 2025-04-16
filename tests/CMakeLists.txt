enable_testing()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

file(
        COPY
            data/
        DESTINATION
            ${CMAKE_CURRENT_BINARY_DIR}/data
)

add_executable(
        tests
        validate_random.cpp
        validate_primality_tests.cpp
        validate_prime_generators.cpp
        validate_factorization.cpp
        test_utils.cpp
)
target_link_libraries(tests big_prime_lib GTest::gtest_main)

gtest_discover_tests(tests)
