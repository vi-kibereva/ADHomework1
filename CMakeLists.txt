cmake_minimum_required(VERSION 4.0)
project(Homework1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Benchmark
        database.cpp
        database.hpp
        benchmark.cpp)

add_executable(Benchmark_sorting
        database.cpp
        database.hpp
        benchmark_sorting.cpp)
