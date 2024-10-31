#!/bin/sh
is_folder_exists() {
    local folder_path="$1"

    if [ -d "$folder_path" ]; then
        return 0
    else
        return 1
    fi
}

is_file_exists() {
    local file_path="$1"

    if [ -e "$file_path" ]; then
        return 0
    else
        return 1
    fi
}

OPENCV_VERSION=4.8.1
CPU_CORES=20

if ! is_folder_exists "./opencv_build"; then
    mkdir ./opencv_build
fi

cd ./opencv_build

if ! is_file_exists "./opencv.zip"; then
    wget -O opencv.zip https://github.com/opencv/opencv/archive/$OPENCV_VERSION.zip
fi
unzip -o opencv.zip

if ! is_file_exists "./opencv_contrib.zip"; then
    wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/$OPENCV_VERSION.zip
fi
unzip -o opencv_contrib.zip

apt install -y build-essential cmake pkg-config unzip yasm git checkinstall
apt install -y libjpeg-dev libpng-dev libtiff-dev
apt install -y libavcodec-dev libavformat-dev libswscale-dev libavresample-dev
apt install -y libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
apt install -y libxvidcore-dev x264 libx264-dev libfaac-dev libmp3lame-dev libtheora-dev
apt install -y libfaac-dev libmp3lame-dev libvorbis-dev
apt install -y libopencore-amrnb-dev libopencore-amrwb-dev
apt-get install -y libdc1394-22 libdc1394-22-dev libxine2-dev libv4l-dev v4l-utils
apt-get install -y libgtk-3-dev
apt-get install -y python3-dev python3-pip
apt install -y python3-testresources
apt-get install -y libtbb-dev
apt-get install -y libatlas-base-dev gfortran
apt-get install -y libprotobuf-dev protobuf-compiler
apt-get install -y libgoogle-glog-dev libgflags-dev
apt-get install -y libgphoto2-dev libeigen3-dev libhdf5-dev doxygen

python3 -m pip install numpy

cd ./opencv-$OPENCV_VERSION

if is_folder_exists "./build"; then
    rm -rf ./build
fi

mkdir ./build
cd ./build

cmake -D CMAKE_BUILD_TYPE=RELEASE \
-D CMAKE_INSTALL_PREFIX=/usr/local \
-D WITH_TBB=ON \
-D ENABLE_FAST_MATH=1 \
-D WITH_QT=OFF \
-D WITH_OPENGL=ON \
-D BUILD_opencv_apps=OFF \
-D BUILD_opencv_python2=OFF \
-D BUILD_opencv_python3=ON \
-D PYTHON3_EXECUTABLE=/usr/bin/python3.8 \
-D OPENCV_PYTHON3_INSTALL_PATH=$(python3 -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())") \
-D PYTHON_DEFAULT_EXECUTABLE=$(which python3) \
-D OPENCV_GENERATE_PKGCONFIG=ON \
-D OPENCV_PC_FILE_NAME=opencv.pc \
-D OPENCV_ENABLE_NONFREE=ON \
-D OPENCV_EXTRA_MODULES_PATH=../../opencv_contrib-$OPENCV_VERSION/modules \
-D WITH_FFMPEG=ON \
-D WITH_GSTREAMER=ON \
-D INSTALL_PYTHON_EXAMPLES=OFF \
-D INSTALL_C_EXAMPLES=OFF \
-D BUILD_EXAMPLES=OFF \
-D BUILD_TESTS=OFF \
-D BUILD_PERF_TESTS=OFF \
..

make -j "$CPU_CORES"

echo "make -j "$CPU_CORES" install"
make -j "$CPU_CORES" install

echo 'export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH' >> ~/.bashrc
echo 'export PYTHONPATH=/usr/local/lib/python3.8/site-packages/:$PYTHONPATH' >> ~/.bashrc
source ~/.bashrc

cd ./python_loader

echo "python3 -m pip install ."
python3 -m pip install . --force-reinstall
