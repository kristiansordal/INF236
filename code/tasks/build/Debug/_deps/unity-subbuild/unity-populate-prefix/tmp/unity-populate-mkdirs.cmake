# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-src"
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-build"
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-subbuild/unity-populate-prefix"
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-subbuild/unity-populate-prefix/tmp"
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp"
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-subbuild/unity-populate-prefix/src"
  "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/Users/kristiansordal/dev/uib/spring24/inf236/code/tasks/build/Debug/_deps/unity-subbuild/unity-populate-prefix/src/unity-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
