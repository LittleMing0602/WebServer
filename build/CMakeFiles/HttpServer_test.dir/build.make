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
include CMakeFiles/HttpServer_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/HttpServer_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HttpServer_test.dir/flags.make

CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o: CMakeFiles/HttpServer_test.dir/flags.make
CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o: ../test/HttpServer_test.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /root/WebServer/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o -c /root/WebServer/test/HttpServer_test.cc

CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/WebServer/test/HttpServer_test.cc > CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.i

CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/WebServer/test/HttpServer_test.cc -o CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.s

CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.requires:
.PHONY : CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.requires

CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.provides: CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.requires
	$(MAKE) -f CMakeFiles/HttpServer_test.dir/build.make CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.provides.build
.PHONY : CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.provides

CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.provides.build: CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o

# Object files for target HttpServer_test
HttpServer_test_OBJECTS = \
"CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o"

# External object files for target HttpServer_test
HttpServer_test_EXTERNAL_OBJECTS =

HttpServer_test: CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o
HttpServer_test: CMakeFiles/HttpServer_test.dir/build.make
HttpServer_test: net/http/libhttp.a
HttpServer_test: net/libnet.a
HttpServer_test: timer/libtimer.a
HttpServer_test: base/libThread.a
HttpServer_test: CMakeFiles/HttpServer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable HttpServer_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HttpServer_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HttpServer_test.dir/build: HttpServer_test
.PHONY : CMakeFiles/HttpServer_test.dir/build

CMakeFiles/HttpServer_test.dir/requires: CMakeFiles/HttpServer_test.dir/test/HttpServer_test.cc.o.requires
.PHONY : CMakeFiles/HttpServer_test.dir/requires

CMakeFiles/HttpServer_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HttpServer_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HttpServer_test.dir/clean

CMakeFiles/HttpServer_test.dir/depend:
	cd /root/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/WebServer /root/WebServer /root/WebServer/build /root/WebServer/build /root/WebServer/build/CMakeFiles/HttpServer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HttpServer_test.dir/depend

