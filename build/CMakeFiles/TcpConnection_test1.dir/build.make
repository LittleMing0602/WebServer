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
include CMakeFiles/TcpConnection_test1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TcpConnection_test1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TcpConnection_test1.dir/flags.make

CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o: CMakeFiles/TcpConnection_test1.dir/flags.make
CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o: ../test/TcpConnection_test1.cc
	$(CMAKE_COMMAND) -E cmake_progress_report /root/WebServer/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o -c /root/WebServer/test/TcpConnection_test1.cc

CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /root/WebServer/test/TcpConnection_test1.cc > CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.i

CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /root/WebServer/test/TcpConnection_test1.cc -o CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.s

CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.requires:
.PHONY : CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.requires

CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.provides: CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.requires
	$(MAKE) -f CMakeFiles/TcpConnection_test1.dir/build.make CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.provides.build
.PHONY : CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.provides

CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.provides.build: CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o

# Object files for target TcpConnection_test1
TcpConnection_test1_OBJECTS = \
"CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o"

# External object files for target TcpConnection_test1
TcpConnection_test1_EXTERNAL_OBJECTS =

TcpConnection_test1: CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o
TcpConnection_test1: CMakeFiles/TcpConnection_test1.dir/build.make
TcpConnection_test1: timer/libtimer.a
TcpConnection_test1: net/libnet.a
TcpConnection_test1: base/libthread.a
TcpConnection_test1: log/liblog.a
TcpConnection_test1: timer/libtimer.a
TcpConnection_test1: base/libthread.a
TcpConnection_test1: CMakeFiles/TcpConnection_test1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable TcpConnection_test1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TcpConnection_test1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TcpConnection_test1.dir/build: TcpConnection_test1
.PHONY : CMakeFiles/TcpConnection_test1.dir/build

CMakeFiles/TcpConnection_test1.dir/requires: CMakeFiles/TcpConnection_test1.dir/test/TcpConnection_test1.cc.o.requires
.PHONY : CMakeFiles/TcpConnection_test1.dir/requires

CMakeFiles/TcpConnection_test1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TcpConnection_test1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TcpConnection_test1.dir/clean

CMakeFiles/TcpConnection_test1.dir/depend:
	cd /root/WebServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/WebServer /root/WebServer /root/WebServer/build /root/WebServer/build /root/WebServer/build/CMakeFiles/TcpConnection_test1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TcpConnection_test1.dir/depend

