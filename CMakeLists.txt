cmake_minimum_required(VERSION 3.29)
project(big_prime_lib)

enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Boost REQUIRED)

add_subdirectory(extern/googletest)

include_directories(src)
file(GLOB src_targets src/**/*.cpp src/**/*.hpp src/*.cpp src/*.hpp)
add_library(
        big_prime_lib
        ${src_targets}
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(big_prime_lib PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_compile_options(big_prime_lib PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,bounds,undefined>)
    target_link_options(big_prime_lib PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,bounds,undefined>)
    target_link_libraries(big_prime_lib PRIVATE $<$<CONFIG:Debug>:-fsanitize=address,bounds,undefined>)
    target_compile_options(big_prime_lib PRIVATE $<$<CONFIG:Release>:-O3>)
elseif (MSVC)
    target_compile_options(big_prime_lib PRIVATE /W4)
endif ()

target_include_directories(big_prime_lib PUBLIC ${Boost_INCLUDE_DIRS})
target_link_libraries(big_prime_lib PRIVATE gmp)

add_subdirectory(example)
add_subdirectory(tests)
