cmake_minimum_required(VERSION 3.20)

project(INF236_Exercises           # Name of the project
    VERSION 0.1.0                  # Version number
    DESCRIPTION "INF236 Exercises" # Description
    LANGUAGES C CXX                # Languages 
)

set(CMAKE_C_STANDARD 11)                  # Language standard for C
set(CMAKE_CXX_STANDARD 20)                # Language standard for C++
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")     # Sonsider adding -O3 -ffast-math as optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Same goes here

find_package(OpenMP REQUIRED) # Add OpenMP
include(CTest)                # For writing tests

# Fetch Unity
include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG v2.5.2  # specify the version you want
)
FetchContent_MakeAvailable(unity)

# Include the Unity source directory
include_directories(${unity_SOURCE_DIR}/src)

# Create executable from source files
add_executable(exercises 
    src/main.c
)

# Executable for tests
add_executable(tests
    src/tests.c 
)


target_compile_options(exercises PRIVATE -Wall -Wextra)   # Compile options with some useful flags. Consider adding -Werror and -Wpedantic
target_link_libraries(exercises PRIVATE OpenMP::OpenMP_C) # Link OpenMP
target_link_libraries(tests PRIVATE unity)                # Link with Unity

add_test(NAME unity_test COMMAND tests)
add_test(NAME TestInstantiator          # Set the name of the test
         COMMAND tests)

set(CMAKE_COLOR_DIAGNOSTIC ON)



