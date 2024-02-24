cmake_minimum_required(VERSION 3.20)

project(Radix_Sort           # Name of the project
    VERSION 0.1.0                  # Version number
    DESCRIPTION "Paralellized Radix Sort" # Description
    LANGUAGES C CXX                # Languages 
)

set(CMAKE_C_STANDARD 11)                  # Language standard for C
set(CMAKE_CXX_STANDARD 20)                # Language standard for C++
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")     # Sonsider adding -O3 -ffast-math as optimization flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}") # Same goes here

find_package(OpenMP REQUIRED) # Add OpenMP

# Create executable from source files
add_executable(radix_seq
    src/radix_seq.h
    src/util.h
    src/mt19937-64.h
    src/mt19937-64.c
    src/radix_seq.c
    src/util.c
    src/main.c
)

add_executable(radix_par
    src/radix_par.h
    src/util.h
    src/mt19937-64.h
    src/mt19937-64.c
    src/radix_par.c
    src/util.c
    src/main.c
)

add_executable(compare
    src/radix_seq.h
    src/radix_par.h
    src/util.h
    src/mt19937-64.h
    src/mt19937-64.c
    src/radix_seq.c
    src/radix_par.c
    src/util.c
    src/main.c
)

target_compile_options(radix_seq PRIVATE -Wall -Wextra)   # Compile options with some useful flags. Consider adding -Werror and -Wpedantic
target_link_libraries(radix_seq PRIVATE OpenMP::OpenMP_C) # Link OpenMP
target_compile_definitions(radix_seq PRIVATE SEQ)

target_compile_options(radix_par PRIVATE -Wall -Wextra)   
target_link_libraries(radix_par PRIVATE OpenMP::OpenMP_C)
target_compile_definitions(radix_par PRIVATE PAR)

target_compile_options(compare PRIVATE -Wall -Wextra)   
target_link_libraries(compare PRIVATE OpenMP::OpenMP_C)
target_compile_definitions(compare PRIVATE COMPARE) # Define TEST to enable testing

set(CMAKE_COLOR_DIAGNOSTIC ON)



