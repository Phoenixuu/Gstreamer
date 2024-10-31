cmake_minimum_required(VERSION 3.2)

project(gst VERSION 1.0.0 DESCRIPTION "gstreamer sample")

# set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti -fPIC -std=c++17 -lstdc++fs")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -pthread")
# set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

option(WAYLAND "WAYLAND" OFF)
if(WAYLAND)
    message(STATUS "WAYLAND: " ${WAYLAND})
    add_definitions(-DWAYLAND)
endif()

find_package(fmt)
find_package(spdlog)
find_package(OpenCV REQUIRED PATHS ${OpenCV_DIR})
find_package(PkgConfig REQUIRED)

pkg_check_modules(GST REQUIRED gstreamer-1.0 gstreamer-app-1.0 gstreamer-audio-1.0 gstreamer-video-1.0)
pkg_check_modules(CAIRO REQUIRED cairo)
pkg_check_modules(FREETYPE2 REQUIRED freetype2)

# ONNX
set(ONNXRUNTIME_DIR ${PROJECT_SOURCE_DIR}/libs/libONNX/onnxruntime-linux-x64-gpu-1.15.1)
find_library(ONNXRUNTIME onnxruntime PATHS ${ONNXRUNTIME_DIR}/lib)
include_directories(${ONNXRUNTIME_DIR}/include)

include_directories(
    ${OpenCV_INCLUDE_DIRS}
    ${GST_INCLUDE_DIRS}
    ${CAIRO_INCLUDE_DIRS}
    ${FREETYPE2_INCLUDE_DIRS}
)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/stream
    ${PROJECT_SOURCE_DIR}/include/model
    ${PROJECT_SOURCE_DIR}/include/model/onnx
)

file(GLOB SOURCE 
    ${PROJECT_SOURCE_DIR}/source/*.cpp
    ${PROJECT_SOURCE_DIR}/source/stream/*.cpp
    ${PROJECT_SOURCE_DIR}/source/model/*.cpp
    ${PROJECT_SOURCE_DIR}/source/model/onnx/*.cpp
)

# add_executable(${PROJECT_NAME} ${SOURCE})
add_library(${PROJECT_NAME} SHARED ${SOURCE})
# add_library(${PROJECT_NAME} STATIC ${SOURCE})

target_link_libraries(${PROJECT_NAME}
    fmt::fmt
    spdlog::spdlog
    ${OpenCV_LIBS}
    ${GST_LIBRARIES}
    ${CAIRO_LIBRARIES}
    ${FREETYPE2_LIBRARIES}
    ${ONNXRUNTIME}
)

add_executable(test ${PROJECT_SOURCE_DIR}/source/main.cpp)
target_link_libraries(test
    ${PROJECT_NAME}
)

add_executable(stream ${PROJECT_SOURCE_DIR}/test/test_stream.cpp)
target_link_libraries(stream
    ${PROJECT_NAME}
)

add_executable(model ${PROJECT_SOURCE_DIR}/test/test_model.cpp)
target_link_libraries(model
    ${PROJECT_NAME}
)
