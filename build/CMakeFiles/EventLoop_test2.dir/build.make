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
include CMakeFiles/EventLoop_test2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/EventLoop_test2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/EventLoop_test2.dir/flags.make

CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o: CMakeFiles/EventLoop_test2.dir/flags.make
CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o: ../test/EventLoop_test2.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /root/WebServer/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o -c /root/WebServer/test/EventLoop_test2.cc

CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/WebServer/test/EventLoop_test2.cc > CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.i

CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/WebServer/test/EventLoop_test2.cc -o CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.s

CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.requires:
.PHONY : CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.requires

CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.provides: CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.requires
	$(MAKE) -f CMakeFiles/EventLoop_test2.dir/build.make CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.provides.build
.PHONY : CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.provides

CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.provides.build: CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o

# Object files for target EventLoop_test2
EventLoop_test2_OBJECTS = \
"CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o"

# External object files for target EventLoop_test2
EventLoop_test2_EXTERNAL_OBJECTS =

EventLoop_test2: CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o
EventLoop_test2: CMakeFiles/EventLoop_test2.dir/build.make
EventLoop_test2: timer/libtimer.a
EventLoop_test2: base/libThread.a
EventLoop_test2: net/libnet.a
EventLoop_test2: timer/libtimer.a
EventLoop_test2: CMakeFiles/EventLoop_test2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable EventLoop_test2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/EventLoop_test2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/EventLoop_test2.dir/build: EventLoop_test2
.PHONY : CMakeFiles/EventLoop_test2.dir/build

CMakeFiles/EventLoop_test2.dir/requires: CMakeFiles/EventLoop_test2.dir/test/EventLoop_test2.cc.o.requires
.PHONY : CMakeFiles/EventLoop_test2.dir/requires

CMakeFiles/EventLoop_test2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/EventLoop_test2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/EventLoop_test2.dir/clean

CMakeFiles/EventLoop_test2.dir/depend:
	cd /root/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/WebServer /root/WebServer /root/WebServer/build /root/WebServer/build /root/WebServer/build/CMakeFiles/EventLoop_test2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/EventLoop_test2.dir/depend

