cmake_minimum_required(VERSION 3.20)

project(INF236_Exercises           # Name of the project
    VERSION 0.1.0                  # Version number
    DESCRIPTION "INF236 Exercises" # Description
    LANGUAGES C CXX                # Languages 
)

set(CMAKE_C_STANDARD 11)                  # Language standard for C
set(CMAKE_CXX_STANDARD 20)                # Language standard for C++
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")     # Consider adding -O3 -ffast-math as optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Same goes here

find_package(OpenMP REQUIRED) # Add OpenMP

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.11.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create executable from source files
add_executable(exercises 
    src/main.c
)

# Executable for week 1 exercises
add_executable(week1
    src/week1/week1.h
    src/week1/week1.c
    src/week1/run.c
)

# Executable for tests
add_executable(tests
    src/tests.cpp 
    src/week1/tests_week1.cpp
    src/week1/week1.c
)
# Executable for tests
add_executable(simd
    src/week2/simd.c
)

target_compile_options(exercises PRIVATE -Wall -Wextra)   # Compile options with some useful flags. Consider adding -Werror and -Wpedantic
target_link_libraries(exercises PRIVATE OpenMP::OpenMP_C) # Link OpenMP
target_link_libraries(simd PRIVATE OpenMP::OpenMP_C) # Link OpenMP
target_link_libraries(tests PRIVATE gtest_main)           # Link with Google Test

enable_testing()
add_test(NAME GoogleTest COMMAND tests)

set(CMAKE_COLOR_DIAGNOSTIC ON)
