# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/iotivity-2.0.1.1-unsecure/json-c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/iotivity-2.0.1.1-unsecure/json-c-build

# Include any dependencies generated for this target.
include tests/CMakeFiles/test_deep_copy.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/test_deep_copy.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/test_deep_copy.dir/flags.make

tests/CMakeFiles/test_deep_copy.dir/test_deep_copy.c.o: tests/CMakeFiles/test_deep_copy.dir/flags.make
tests/CMakeFiles/test_deep_copy.dir/test_deep_copy.c.o: /home/pi/iotivity-2.0.1.1-unsecure/json-c/tests/test_deep_copy.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/iotivity-2.0.1.1-unsecure/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object tests/CMakeFiles/test_deep_copy.dir/test_deep_copy.c.o"
	cd /home/pi/iotivity-2.0.1.1-unsecure/json-c-build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/test_deep_copy.dir/test_deep_copy.c.o   -c /home/pi/iotivity-2.0.1.1-unsecure/json-c/tests/test_deep_copy.c

tests/CMakeFiles/test_deep_copy.dir/test_deep_copy.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/test_deep_copy.dir/test_deep_copy.c.i"
	cd /home/pi/iotivity-2.0.1.1-unsecure/json-c-build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/pi/iotivity-2.0.1.1-unsecure/json-c/tests/test_deep_copy.c > CMakeFiles/test_deep_copy.dir/test_deep_copy.c.i

tests/CMakeFiles/test_deep_copy.dir/test_deep_copy.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/test_deep_copy.dir/test_deep_copy.c.s"
	cd /home/pi/iotivity-2.0.1.1-unsecure/json-c-build/tests && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/pi/iotivity-2.0.1.1-unsecure/json-c/tests/test_deep_copy.c -o CMakeFiles/test_deep_copy.dir/test_deep_copy.c.s

# Object files for target test_deep_copy
test_deep_copy_OBJECTS = \
"CMakeFiles/test_deep_copy.dir/test_deep_copy.c.o"

# External object files for target test_deep_copy
test_deep_copy_EXTERNAL_OBJECTS =

tests/test_deep_copy: tests/CMakeFiles/test_deep_copy.dir/test_deep_copy.c.o
tests/test_deep_copy: tests/CMakeFiles/test_deep_copy.dir/build.make
tests/test_deep_copy: libjson-c.so.5.1.0
tests/test_deep_copy: tests/CMakeFiles/test_deep_copy.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/iotivity-2.0.1.1-unsecure/json-c-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable test_deep_copy"
	cd /home/pi/iotivity-2.0.1.1-unsecure/json-c-build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_deep_copy.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/test_deep_copy.dir/build: tests/test_deep_copy

.PHONY : tests/CMakeFiles/test_deep_copy.dir/build

tests/CMakeFiles/test_deep_copy.dir/clean:
	cd /home/pi/iotivity-2.0.1.1-unsecure/json-c-build/tests && $(CMAKE_COMMAND) -P CMakeFiles/test_deep_copy.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/test_deep_copy.dir/clean

tests/CMakeFiles/test_deep_copy.dir/depend:
	cd /home/pi/iotivity-2.0.1.1-unsecure/json-c-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/iotivity-2.0.1.1-unsecure/json-c /home/pi/iotivity-2.0.1.1-unsecure/json-c/tests /home/pi/iotivity-2.0.1.1-unsecure/json-c-build /home/pi/iotivity-2.0.1.1-unsecure/json-c-build/tests /home/pi/iotivity-2.0.1.1-unsecure/json-c-build/tests/CMakeFiles/test_deep_copy.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/test_deep_copy.dir/depend
