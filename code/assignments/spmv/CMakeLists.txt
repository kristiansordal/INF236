cmake_minimum_required(VERSION 3.29.2)

project(SpMV
    VERSION 0.0.1
    DESCRIPTION "Sparse Matrix Multiplication"
    LANGUAGES CXX
)

if(CMAKE_VERSION VERSION_LESS "3.1")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    set(CMAKE_CXX_STANDARD 11)
endif()

# Add METIS include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Set the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find OpenMP
find_package(OpenMP REQUIRED)

# Find METIS
find_package(METIS REQUIRED)

# List of all executables
set(all_executables
    shared
    seq
    naive
)

# Adding executables
foreach(executable IN LISTS all_executables)
    add_executable(${executable} 
    src/main_${executable}.cpp
    src/spmv.cpp
    src/spmv.hpp
    )
    target_include_directories(${executable} PRIVATE ${METIS_INCLUDE_DIRS})
    target_link_libraries(${executable} PRIVATE ${METIS_LIBRARIES})
endforeach()

foreach(executable IN LISTS all_executables)
    if(OPENMP_FOUND)
        set_target_properties(${executable} PROPERTIES
            COMPILE_FLAGS "${OpenMP_CXX_FLAGS} -fsanitize=address -O3"
            LINK_FLAGS "${OpenMP_CXX_FLAGS} -fsanitize=address"
        )
    endif()
endforeach()

set(CMAKE_COLOR_DIAGNOSTICS On)
