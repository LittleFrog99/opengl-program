# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/littlefrog/opengl-program

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/littlefrog/opengl-program/bin

# Include any dependencies generated for this target.
include CMakeFiles/OpenGLProgram.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/OpenGLProgram.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/OpenGLProgram.dir/flags.make

CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o: CMakeFiles/OpenGLProgram.dir/flags.make
CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o: ../src/terrain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/littlefrog/opengl-program/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o -c /Users/littlefrog/opengl-program/src/terrain.cpp

CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/littlefrog/opengl-program/src/terrain.cpp > CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.i

CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/littlefrog/opengl-program/src/terrain.cpp -o CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.s

CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.requires:

.PHONY : CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.requires

CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.provides: CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.requires
	$(MAKE) -f CMakeFiles/OpenGLProgram.dir/build.make CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.provides.build
.PHONY : CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.provides

CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.provides.build: CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o


# Object files for target OpenGLProgram
OpenGLProgram_OBJECTS = \
"CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o"

# External object files for target OpenGLProgram
OpenGLProgram_EXTERNAL_OBJECTS =

OpenGLProgram: CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o
OpenGLProgram: CMakeFiles/OpenGLProgram.dir/build.make
OpenGLProgram: CMakeFiles/OpenGLProgram.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/littlefrog/opengl-program/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable OpenGLProgram"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OpenGLProgram.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/OpenGLProgram.dir/build: OpenGLProgram

.PHONY : CMakeFiles/OpenGLProgram.dir/build

CMakeFiles/OpenGLProgram.dir/requires: CMakeFiles/OpenGLProgram.dir/src/terrain.cpp.o.requires

.PHONY : CMakeFiles/OpenGLProgram.dir/requires

CMakeFiles/OpenGLProgram.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/OpenGLProgram.dir/cmake_clean.cmake
.PHONY : CMakeFiles/OpenGLProgram.dir/clean

CMakeFiles/OpenGLProgram.dir/depend:
	cd /Users/littlefrog/opengl-program/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/littlefrog/opengl-program /Users/littlefrog/opengl-program /Users/littlefrog/opengl-program/bin /Users/littlefrog/opengl-program/bin /Users/littlefrog/opengl-program/bin/CMakeFiles/OpenGLProgram.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/OpenGLProgram.dir/depend

