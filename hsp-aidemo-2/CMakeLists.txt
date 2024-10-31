cmake_minimum_required(VERSION 3.10)

PROJECT(aidemo LANGUAGES CXX)

include_directories(${SYSROOT_INCDIR})
link_directories(${SYSROOT_LIBDIR})

if(BUILD_X86)
    set(CMAKE_C_COMPILER /usr/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/bin/clang++)
elseif(BUILD_AARCH64)
    set(CMAKE_C_COMPILER /usr/bin/aarch64-linux-gnu-gcc-9)
    set(CMAKE_CXX_COMPILER /usr/bin/aarch64-linux-gnu-g++-9)
endif()

set(CMAKE_BUILD_TYPE Debug)
if(BUILD_X86)
    set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/libs/opencv-4.9.0/x86)
elseif(BUILD_AARCH64)
    set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/libs/opencv-4.9.0/aarch64)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Ofast -Wfatal-errors -fexceptions")

INCLUDE_DIRECTORIES(
    ${OpenCV_DIR}/include/opencv4
    ${CMAKE_SOURCE_DIR}/include/nlohmann
    ${CMAKE_SOURCE_DIR}/include/glib-2.0
    ${CMAKE_SOURCE_DIR}/include/gstreamer-1.0
    ${CMAKE_SOURCE_DIR}/include/SNPE2.19
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/core
)

file (GLOB SRC_FILES 
    ${CMAKE_SOURCE_DIR}/src/core/*.cpp
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

if(BUILD_X86)
    LINK_DIRECTORIES(${PROJECT_NAME}
        ${OpenCV_DIR}/lib
        ${CMAKE_SOURCE_DIR}/libs/SNPE2.19/x86_64-linux-clang
    )
elseif(BUILD_AARCH64)
    LINK_DIRECTORIES(${PROJECT_NAME}
        ${OpenCV_DIR}/lib
        ${CMAKE_SOURCE_DIR}/libs/SNPE2.19/aarch64-ubuntu-gcc9.4
    )
endif()

ADD_EXECUTABLE(${PROJECT_NAME} ${SRC_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
    -lm -ldl -lpthread -lcairo
    -lopencv_core
    -lopencv_imgproc
    -lopencv_imgcodecs
    -lopencv_videoio
    -lopencv_highgui
    -lSNPE
    /usr/lib/aarch64-linux-gnu/libgstreamer-1.0.so.0.1603.0
    /usr/lib/aarch64-linux-gnu/libgstapp-1.0.so.0.1603.0
    /usr/lib/aarch64-linux-gnu/libglib-2.0.so.0.6400.6
    /usr/lib/aarch64-linux-gnu/libgobject-2.0.so.0.6400.6
) 

add_definitions(-O2 -pthread -frtti)
