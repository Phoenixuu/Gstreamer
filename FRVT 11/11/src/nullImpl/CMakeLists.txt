project(nullImpl)
cmake_minimum_required(VERSION 2.8)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../include ${CMAKE_CURRENT_SOURCE_DIR}/../../../common/src/include)

set(ncnn_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/ncnn/lib/cmake/ncnn)
find_package (ncnn REQUIRED) 

# Configure built shared libraries in top-level lib directory
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../../lib)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../lib/opencv/include/opencv4)

# # Đường dẫn tới thư mục chứa các tệp tin thư viện .so
# set(LIBRARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/opencv/lib)

# # Tìm tất cả các tệp tin thư viện .so trong thư mục
# file(GLOB LIBRARY_FILES
#     ${LIBRARY_DIR}/opencv4/3rdparty/*.a
#     ${LIBRARY_DIR}/*.a)
# message(${LIBRARY_FILES})

set(OpenCV_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/opencv/lib/cmake/opencv4)
find_package (OpenCV REQUIRED) 

LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/../../lib)

include_directories($ENV{CMAKE_CURRENT_SOURCE_DIR}/../../lib/eigen3)
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

# Build the shared libraries
# set (CMAKE_SHARED_LINKER_FLAGS "-Wl,-rpath,$CMAKE_LIBRARY_OUTPUT_DIRECTORY")
add_library (frvt_11_ncsgroup_000 SHARED nullimplfrvt11.cpp FaceWarp.cpp FaceDetection.cpp FaceVectorize.cpp)
target_link_libraries (frvt_11_ncsgroup_000 ${OpenCV_LIBS} ncnn)