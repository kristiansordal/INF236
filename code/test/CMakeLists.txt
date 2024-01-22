cmake_minimum_required(VERSION 3.20)


set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


project(testlib LANGUAGES CXX C)

add_executable(main
src/main.c
)

