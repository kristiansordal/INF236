# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.28.0/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug

# Include any dependencies generated for this target.
include CMakeFiles/tests.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/tests.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/tests.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tests.dir/flags.make

CMakeFiles/tests.dir/src/tests.c.o: CMakeFiles/tests.dir/flags.make
CMakeFiles/tests.dir/src/tests.c.o: /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/src/tests.c
CMakeFiles/tests.dir/src/tests.c.o: CMakeFiles/tests.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/tests.dir/src/tests.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/tests.dir/src/tests.c.o -MF CMakeFiles/tests.dir/src/tests.c.o.d -o CMakeFiles/tests.dir/src/tests.c.o -c /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/src/tests.c

CMakeFiles/tests.dir/src/tests.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/tests.dir/src/tests.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/src/tests.c > CMakeFiles/tests.dir/src/tests.c.i

CMakeFiles/tests.dir/src/tests.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/tests.dir/src/tests.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/src/tests.c -o CMakeFiles/tests.dir/src/tests.c.s

# Object files for target tests
tests_OBJECTS = \
"CMakeFiles/tests.dir/src/tests.c.o"

# External object files for target tests
tests_EXTERNAL_OBJECTS =

tests: CMakeFiles/tests.dir/src/tests.c.o
tests: CMakeFiles/tests.dir/build.make
tests: _deps/unity-build/libunity.a
tests: CMakeFiles/tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable tests"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tests.dir/build: tests
.PHONY : CMakeFiles/tests.dir/build

CMakeFiles/tests.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tests.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tests.dir/clean

CMakeFiles/tests.dir/depend:
	cd /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug /Users/kristiansordal/dev/uib/spring24/inf236/code/assignments/build/Debug/CMakeFiles/tests.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/tests.dir/depend

