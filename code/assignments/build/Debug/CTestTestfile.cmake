# CMake generated Testfile for 
# Source directory: /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments
# Build directory: /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test([=[unity_test]=] "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug/tests")
set_tests_properties([=[unity_test]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/CMakeLists.txt;44;add_test;/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/CMakeLists.txt;0;")
add_test([=[TestInstantiator]=] "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug/tests")
set_tests_properties([=[TestInstantiator]=] PROPERTIES  _BACKTRACE_TRIPLES "/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/CMakeLists.txt;45;add_test;/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/CMakeLists.txt;0;")
subdirs("_deps/unity-build")
