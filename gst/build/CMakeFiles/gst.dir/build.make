# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dunggps/folder/gst

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dunggps/folder/gst/build

# Include any dependencies generated for this target.
include CMakeFiles/gst.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gst.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gst.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gst.dir/flags.make

CMakeFiles/gst.dir/source/main.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/main.cpp.o: ../source/main.cpp
CMakeFiles/gst.dir/source/main.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gst.dir/source/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/main.cpp.o -MF CMakeFiles/gst.dir/source/main.cpp.o.d -o CMakeFiles/gst.dir/source/main.cpp.o -c /home/dunggps/folder/gst/source/main.cpp

CMakeFiles/gst.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/main.cpp > CMakeFiles/gst.dir/source/main.cpp.i

CMakeFiles/gst.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/main.cpp -o CMakeFiles/gst.dir/source/main.cpp.s

CMakeFiles/gst.dir/source/model/AIModel.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/model/AIModel.cpp.o: ../source/model/AIModel.cpp
CMakeFiles/gst.dir/source/model/AIModel.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/gst.dir/source/model/AIModel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/model/AIModel.cpp.o -MF CMakeFiles/gst.dir/source/model/AIModel.cpp.o.d -o CMakeFiles/gst.dir/source/model/AIModel.cpp.o -c /home/dunggps/folder/gst/source/model/AIModel.cpp

CMakeFiles/gst.dir/source/model/AIModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/model/AIModel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/model/AIModel.cpp > CMakeFiles/gst.dir/source/model/AIModel.cpp.i

CMakeFiles/gst.dir/source/model/AIModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/model/AIModel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/model/AIModel.cpp -o CMakeFiles/gst.dir/source/model/AIModel.cpp.s

CMakeFiles/gst.dir/source/model/ImageModel.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/model/ImageModel.cpp.o: ../source/model/ImageModel.cpp
CMakeFiles/gst.dir/source/model/ImageModel.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/gst.dir/source/model/ImageModel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/model/ImageModel.cpp.o -MF CMakeFiles/gst.dir/source/model/ImageModel.cpp.o.d -o CMakeFiles/gst.dir/source/model/ImageModel.cpp.o -c /home/dunggps/folder/gst/source/model/ImageModel.cpp

CMakeFiles/gst.dir/source/model/ImageModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/model/ImageModel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/model/ImageModel.cpp > CMakeFiles/gst.dir/source/model/ImageModel.cpp.i

CMakeFiles/gst.dir/source/model/ImageModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/model/ImageModel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/model/ImageModel.cpp -o CMakeFiles/gst.dir/source/model/ImageModel.cpp.s

CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o: ../source/model/RuntimeFactory.cpp
CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o -MF CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o.d -o CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o -c /home/dunggps/folder/gst/source/model/RuntimeFactory.cpp

CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/model/RuntimeFactory.cpp > CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.i

CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/model/RuntimeFactory.cpp -o CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.s

CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o: ../source/model/onnx/ORTModel.cpp
CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o -MF CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o.d -o CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o -c /home/dunggps/folder/gst/source/model/onnx/ORTModel.cpp

CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/model/onnx/ORTModel.cpp > CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.i

CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/model/onnx/ORTModel.cpp -o CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.s

CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o: ../source/model/onnx/ORTRunner.cpp
CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o -MF CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o.d -o CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o -c /home/dunggps/folder/gst/source/model/onnx/ORTRunner.cpp

CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/model/onnx/ORTRunner.cpp > CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.i

CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/model/onnx/ORTRunner.cpp -o CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.s

CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o: ../source/model/onnx/ORTYoloV8.cpp
CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o -MF CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o.d -o CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o -c /home/dunggps/folder/gst/source/model/onnx/ORTYoloV8.cpp

CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/model/onnx/ORTYoloV8.cpp > CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.i

CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/model/onnx/ORTYoloV8.cpp -o CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.s

CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o: ../source/stream/GstDecode.cpp
CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o -MF CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o.d -o CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o -c /home/dunggps/folder/gst/source/stream/GstDecode.cpp

CMakeFiles/gst.dir/source/stream/GstDecode.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/stream/GstDecode.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/stream/GstDecode.cpp > CMakeFiles/gst.dir/source/stream/GstDecode.cpp.i

CMakeFiles/gst.dir/source/stream/GstDecode.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/stream/GstDecode.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/stream/GstDecode.cpp -o CMakeFiles/gst.dir/source/stream/GstDecode.cpp.s

CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o: ../source/stream/GstOverlay.cpp
CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o -MF CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o.d -o CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o -c /home/dunggps/folder/gst/source/stream/GstOverlay.cpp

CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/stream/GstOverlay.cpp > CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.i

CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/stream/GstOverlay.cpp -o CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.s

CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o: ../source/stream/GstRestream.cpp
CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o -MF CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o.d -o CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o -c /home/dunggps/folder/gst/source/stream/GstRestream.cpp

CMakeFiles/gst.dir/source/stream/GstRestream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/stream/GstRestream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/stream/GstRestream.cpp > CMakeFiles/gst.dir/source/stream/GstRestream.cpp.i

CMakeFiles/gst.dir/source/stream/GstRestream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/stream/GstRestream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/stream/GstRestream.cpp -o CMakeFiles/gst.dir/source/stream/GstRestream.cpp.s

CMakeFiles/gst.dir/source/stream/Stream.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/stream/Stream.cpp.o: ../source/stream/Stream.cpp
CMakeFiles/gst.dir/source/stream/Stream.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/gst.dir/source/stream/Stream.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/stream/Stream.cpp.o -MF CMakeFiles/gst.dir/source/stream/Stream.cpp.o.d -o CMakeFiles/gst.dir/source/stream/Stream.cpp.o -c /home/dunggps/folder/gst/source/stream/Stream.cpp

CMakeFiles/gst.dir/source/stream/Stream.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/stream/Stream.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/stream/Stream.cpp > CMakeFiles/gst.dir/source/stream/Stream.cpp.i

CMakeFiles/gst.dir/source/stream/Stream.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/stream/Stream.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/stream/Stream.cpp -o CMakeFiles/gst.dir/source/stream/Stream.cpp.s

CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o: CMakeFiles/gst.dir/flags.make
CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o: ../source/stream/StreamManager.cpp
CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o: CMakeFiles/gst.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o -MF CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o.d -o CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o -c /home/dunggps/folder/gst/source/stream/StreamManager.cpp

CMakeFiles/gst.dir/source/stream/StreamManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gst.dir/source/stream/StreamManager.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/dunggps/folder/gst/source/stream/StreamManager.cpp > CMakeFiles/gst.dir/source/stream/StreamManager.cpp.i

CMakeFiles/gst.dir/source/stream/StreamManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gst.dir/source/stream/StreamManager.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/dunggps/folder/gst/source/stream/StreamManager.cpp -o CMakeFiles/gst.dir/source/stream/StreamManager.cpp.s

# Object files for target gst
gst_OBJECTS = \
"CMakeFiles/gst.dir/source/main.cpp.o" \
"CMakeFiles/gst.dir/source/model/AIModel.cpp.o" \
"CMakeFiles/gst.dir/source/model/ImageModel.cpp.o" \
"CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o" \
"CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o" \
"CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o" \
"CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o" \
"CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o" \
"CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o" \
"CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o" \
"CMakeFiles/gst.dir/source/stream/Stream.cpp.o" \
"CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o"

# External object files for target gst
gst_EXTERNAL_OBJECTS =

libgst.so: CMakeFiles/gst.dir/source/main.cpp.o
libgst.so: CMakeFiles/gst.dir/source/model/AIModel.cpp.o
libgst.so: CMakeFiles/gst.dir/source/model/ImageModel.cpp.o
libgst.so: CMakeFiles/gst.dir/source/model/RuntimeFactory.cpp.o
libgst.so: CMakeFiles/gst.dir/source/model/onnx/ORTModel.cpp.o
libgst.so: CMakeFiles/gst.dir/source/model/onnx/ORTRunner.cpp.o
libgst.so: CMakeFiles/gst.dir/source/model/onnx/ORTYoloV8.cpp.o
libgst.so: CMakeFiles/gst.dir/source/stream/GstDecode.cpp.o
libgst.so: CMakeFiles/gst.dir/source/stream/GstOverlay.cpp.o
libgst.so: CMakeFiles/gst.dir/source/stream/GstRestream.cpp.o
libgst.so: CMakeFiles/gst.dir/source/stream/Stream.cpp.o
libgst.so: CMakeFiles/gst.dir/source/stream/StreamManager.cpp.o
libgst.so: CMakeFiles/gst.dir/build.make
libgst.so: /usr/lib/x86_64-linux-gnu/libspdlog.so.1.9.2
libgst.so: /usr/local/lib/libopencv_gapi.so.4.10.0
libgst.so: /usr/local/lib/libopencv_stitching.so.4.10.0
libgst.so: /usr/local/lib/libopencv_aruco.so.4.10.0
libgst.so: /usr/local/lib/libopencv_bgsegm.so.4.10.0
libgst.so: /usr/local/lib/libopencv_bioinspired.so.4.10.0
libgst.so: /usr/local/lib/libopencv_ccalib.so.4.10.0
libgst.so: /usr/local/lib/libopencv_dnn_objdetect.so.4.10.0
libgst.so: /usr/local/lib/libopencv_dnn_superres.so.4.10.0
libgst.so: /usr/local/lib/libopencv_dpm.so.4.10.0
libgst.so: /usr/local/lib/libopencv_face.so.4.10.0
libgst.so: /usr/local/lib/libopencv_freetype.so.4.10.0
libgst.so: /usr/local/lib/libopencv_fuzzy.so.4.10.0
libgst.so: /usr/local/lib/libopencv_hfs.so.4.10.0
libgst.so: /usr/local/lib/libopencv_img_hash.so.4.10.0
libgst.so: /usr/local/lib/libopencv_intensity_transform.so.4.10.0
libgst.so: /usr/local/lib/libopencv_line_descriptor.so.4.10.0
libgst.so: /usr/local/lib/libopencv_mcc.so.4.10.0
libgst.so: /usr/local/lib/libopencv_quality.so.4.10.0
libgst.so: /usr/local/lib/libopencv_rapid.so.4.10.0
libgst.so: /usr/local/lib/libopencv_reg.so.4.10.0
libgst.so: /usr/local/lib/libopencv_rgbd.so.4.10.0
libgst.so: /usr/local/lib/libopencv_saliency.so.4.10.0
libgst.so: /usr/local/lib/libopencv_signal.so.4.10.0
libgst.so: /usr/local/lib/libopencv_stereo.so.4.10.0
libgst.so: /usr/local/lib/libopencv_structured_light.so.4.10.0
libgst.so: /usr/local/lib/libopencv_superres.so.4.10.0
libgst.so: /usr/local/lib/libopencv_surface_matching.so.4.10.0
libgst.so: /usr/local/lib/libopencv_tracking.so.4.10.0
libgst.so: /usr/local/lib/libopencv_videostab.so.4.10.0
libgst.so: /usr/local/lib/libopencv_wechat_qrcode.so.4.10.0
libgst.so: /usr/local/lib/libopencv_xfeatures2d.so.4.10.0
libgst.so: /usr/local/lib/libopencv_xobjdetect.so.4.10.0
libgst.so: /usr/local/lib/libopencv_xphoto.so.4.10.0
libgst.so: ../libs/libONNX/onnxruntime-linux-x64-gpu-1.15.1/lib/libonnxruntime.so
libgst.so: /usr/lib/x86_64-linux-gnu/libfmt.so.8.1.1
libgst.so: /usr/local/lib/libopencv_shape.so.4.10.0
libgst.so: /usr/local/lib/libopencv_highgui.so.4.10.0
libgst.so: /usr/local/lib/libopencv_datasets.so.4.10.0
libgst.so: /usr/local/lib/libopencv_plot.so.4.10.0
libgst.so: /usr/local/lib/libopencv_text.so.4.10.0
libgst.so: /usr/local/lib/libopencv_ml.so.4.10.0
libgst.so: /usr/local/lib/libopencv_phase_unwrapping.so.4.10.0
libgst.so: /usr/local/lib/libopencv_optflow.so.4.10.0
libgst.so: /usr/local/lib/libopencv_ximgproc.so.4.10.0
libgst.so: /usr/local/lib/libopencv_video.so.4.10.0
libgst.so: /usr/local/lib/libopencv_videoio.so.4.10.0
libgst.so: /usr/local/lib/libopencv_imgcodecs.so.4.10.0
libgst.so: /usr/local/lib/libopencv_objdetect.so.4.10.0
libgst.so: /usr/local/lib/libopencv_calib3d.so.4.10.0
libgst.so: /usr/local/lib/libopencv_dnn.so.4.10.0
libgst.so: /usr/local/lib/libopencv_features2d.so.4.10.0
libgst.so: /usr/local/lib/libopencv_flann.so.4.10.0
libgst.so: /usr/local/lib/libopencv_photo.so.4.10.0
libgst.so: /usr/local/lib/libopencv_imgproc.so.4.10.0
libgst.so: /usr/local/lib/libopencv_core.so.4.10.0
libgst.so: CMakeFiles/gst.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/dunggps/folder/gst/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Linking CXX shared library libgst.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gst.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gst.dir/build: libgst.so
.PHONY : CMakeFiles/gst.dir/build

CMakeFiles/gst.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gst.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gst.dir/clean

CMakeFiles/gst.dir/depend:
	cd /home/dunggps/folder/gst/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dunggps/folder/gst /home/dunggps/folder/gst /home/dunggps/folder/gst/build /home/dunggps/folder/gst/build /home/dunggps/folder/gst/build/CMakeFiles/gst.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gst.dir/depend
