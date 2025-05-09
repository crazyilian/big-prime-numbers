enable_testing()

find_package(Threads REQUIRED)
find_package(benchmark REQUIRED)

file(
        COPY
            ../data/
        DESTINATION
            ${CMAKE_CURRENT_BINARY_DIR}/data
)


file(GLOB bench_targets *.cpp)
add_executable(benchmarks ${bench_targets} ../test_utils.cpp)

target_link_libraries(benchmarks
        PRIVATE
        big_prime_lib
        benchmark::benchmark
        benchmark::benchmark_main
        ${CMAKE_THREAD_LIBS_INIT}
)

add_test(
        NAME benchmarks_noiseless_csv
        COMMAND $<TARGET_FILE:benchmarks>
        --benchmark_repetitions=2
        --benchmark_out_format=csv
        --benchmark_out=benchmarks.csv
)
