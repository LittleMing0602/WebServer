# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/WebServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/WebServer/build

# Include any dependencies generated for this target.
include CMakeFiles/Send_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Send_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Send_test.dir/flags.make

CMakeFiles/Send_test.dir/test/Send_Test.cc.o: CMakeFiles/Send_test.dir/flags.make
CMakeFiles/Send_test.dir/test/Send_Test.cc.o: ../test/Send_Test.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /root/WebServer/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/Send_test.dir/test/Send_Test.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/Send_test.dir/test/Send_Test.cc.o -c /root/WebServer/test/Send_Test.cc

CMakeFiles/Send_test.dir/test/Send_Test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Send_test.dir/test/Send_Test.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/WebServer/test/Send_Test.cc > CMakeFiles/Send_test.dir/test/Send_Test.cc.i

CMakeFiles/Send_test.dir/test/Send_Test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Send_test.dir/test/Send_Test.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/WebServer/test/Send_Test.cc -o CMakeFiles/Send_test.dir/test/Send_Test.cc.s

CMakeFiles/Send_test.dir/test/Send_Test.cc.o.requires:
.PHONY : CMakeFiles/Send_test.dir/test/Send_Test.cc.o.requires

CMakeFiles/Send_test.dir/test/Send_Test.cc.o.provides: CMakeFiles/Send_test.dir/test/Send_Test.cc.o.requires
	$(MAKE) -f CMakeFiles/Send_test.dir/build.make CMakeFiles/Send_test.dir/test/Send_Test.cc.o.provides.build
.PHONY : CMakeFiles/Send_test.dir/test/Send_Test.cc.o.provides

CMakeFiles/Send_test.dir/test/Send_Test.cc.o.provides.build: CMakeFiles/Send_test.dir/test/Send_Test.cc.o

# Object files for target Send_test
Send_test_OBJECTS = \
"CMakeFiles/Send_test.dir/test/Send_Test.cc.o"

# External object files for target Send_test
Send_test_EXTERNAL_OBJECTS =

Send_test: CMakeFiles/Send_test.dir/test/Send_Test.cc.o
Send_test: CMakeFiles/Send_test.dir/build.make
Send_test: net/libnet.a
Send_test: timer/libtimer.a
Send_test: CMakeFiles/Send_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable Send_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Send_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Send_test.dir/build: Send_test
.PHONY : CMakeFiles/Send_test.dir/build

CMakeFiles/Send_test.dir/requires: CMakeFiles/Send_test.dir/test/Send_Test.cc.o.requires
.PHONY : CMakeFiles/Send_test.dir/requires

CMakeFiles/Send_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Send_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Send_test.dir/clean

CMakeFiles/Send_test.dir/depend:
	cd /root/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/WebServer /root/WebServer /root/WebServer/build /root/WebServer/build /root/WebServer/build/CMakeFiles/Send_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Send_test.dir/depend
