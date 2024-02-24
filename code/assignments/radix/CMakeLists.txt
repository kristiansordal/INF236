cmake_minimum_required(VERSION 3.20)

project(Radix_Sort           # Name of the project
    VERSION 0.1.0                  # Version number
    DESCRIPTION "Paralellized Radix Sort" # Description
    LANGUAGES C                # Languages 
)

set(CMAKE_C_STANDARD 11)                  # Language standard for C
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")     # Sonsider adding -O3 -ffast-math as optimization flags

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

foreach(target radix_seq radix_par compare)
    target_compile_options(${target} PRIVATE -Wall -Wextra)
    target_link_options(${target} PRIVATE -fsanitize=address) # Link with AddressSanitizer
    target_link_libraries(${target} PRIVATE OpenMP::OpenMP_C)

    # Set the correct definition for each target
    if(${target} STREQUAL "radix_seq")
        target_compile_definitions(${target} PRIVATE SEQ)
    elseif(${target} STREQUAL "radix_par")
        target_compile_definitions(${target} PRIVATE PAR)
    elseif(${target} STREQUAL "compare")
        target_compile_definitions(${target} PRIVATE COMPARE)
    endif()
endforeach()

set(CMAKE_COLOR_DIAGNOSTIC ON)



