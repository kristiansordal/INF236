# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

if(EXISTS "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitclone-lastrun.txt" AND EXISTS "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitinfo.txt" AND
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitclone-lastrun.txt" IS_NEWER_THAN "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitinfo.txt")
  message(STATUS
    "Avoiding repeated git clone, stamp file is up to date: "
    "'/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitclone-lastrun.txt'"
  )
  return()
endif()

execute_process(
  COMMAND ${CMAKE_COMMAND} -E rm -rf "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to remove directory: '/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-src'")
endif()

# try the clone 3 times in case there is an odd git clone issue
set(error_code 1)
set(number_of_tries 0)
while(error_code AND number_of_tries LESS 3)
  execute_process(
    COMMAND "/opt/homebrew/bin/git"
            clone --no-checkout --depth 1 --no-single-branch --config "advice.detachedHead=false" "https://github.com/alugowski/fast_matrix_market" "fast_matrix_market-src"
    WORKING_DIRECTORY "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps"
    RESULT_VARIABLE error_code
  )
  math(EXPR number_of_tries "${number_of_tries} + 1")
endwhile()
if(number_of_tries GREATER 1)
  message(STATUS "Had to git clone more than once: ${number_of_tries} times.")
endif()
if(error_code)
  message(FATAL_ERROR "Failed to clone repository: 'https://github.com/alugowski/fast_matrix_market'")
endif()

execute_process(
  COMMAND "/opt/homebrew/bin/git"
          checkout "main" --
  WORKING_DIRECTORY "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-src"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to checkout tag: 'main'")
endif()

set(init_submodules TRUE)
if(init_submodules)
  execute_process(
    COMMAND "/opt/homebrew/bin/git" 
            submodule update --recursive --init 
    WORKING_DIRECTORY "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-src"
    RESULT_VARIABLE error_code
  )
endif()
if(error_code)
  message(FATAL_ERROR "Failed to update submodules in: '/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-src'")
endif()

# Complete success, update the script-last-run stamp file:
#
execute_process(
  COMMAND ${CMAKE_COMMAND} -E copy "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitinfo.txt" "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitclone-lastrun.txt"
  RESULT_VARIABLE error_code
)
if(error_code)
  message(FATAL_ERROR "Failed to copy script-last-run stamp file: '/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/spmv/build/Debug/_deps/fast_matrix_market-subbuild/fast_matrix_market-populate-prefix/src/fast_matrix_market-populate-stamp/fast_matrix_market-populate-gitclone-lastrun.txt'")
endif()
