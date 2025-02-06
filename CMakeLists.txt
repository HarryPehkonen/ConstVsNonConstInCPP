cmake_minimum_required(VERSION 3.10)
project(const_benchmark CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set release build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Explicitly set optimization flags
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Find required packages
find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)

# Add executable
add_executable(const_benchmark src/benchmark.cpp)

# Link libraries
target_link_libraries(const_benchmark
    PRIVATE
        benchmark::benchmark
        Threads::Threads
)

# Enable warnings and optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(const_benchmark 
        PRIVATE 
            -Wall 
            -Wextra 
            -O3
    )
endif()

# Print the flags being used
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")
