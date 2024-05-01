cmake_minimum_required(VERSION 2.8)

project(SpMV)

# Set C++ standard via compiler flags
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
endif()

# Set include directory
include_directories(${CMAKE_SOURCE_DIR}/include)

# Set the module path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Find OpenMP
set(OpenMP_CXX_FLAGS "-fopenmp")
set(OpenMP_CXX_LIB_NAMES "gomp")
# find_package(OpenMP REQUIRED)

# List of all executables
set(all_executables
    shared
    seq
)

# Adding executables
foreach(executable IN LISTS all_executables)
    add_executable(${executable} 
    src/main_${executable}.cpp
    src/spmv.cpp
    src/spmv.hpp
    )
endforeach()

foreach(executable IN LISTS all_executables)
    include_directories(${executable} ${METIS_INCLUDE_DIRS})
    target_link_libraries(${executable} 
        ${METIS_LIBRARIES}
        OpenMP::OpenMP_CXX
    )
    set_target_properties(${executable} PROPERTIES
        COMPILE_FLAGS "-fsanitize=address -O3"
        LINK_FLAGS "-fsanitize=address"
    )
endforeach()
