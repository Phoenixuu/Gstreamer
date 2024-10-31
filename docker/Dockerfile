FROM arm64v8/ubuntu:20.04
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1
ENV DEBIAN_FRONTEND=noninteractive
ENV DISPLAY=:1
ENV TZ=Asia/Ho_Chi_Minh
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update
RUN apt-get install -y ffmpeg
RUN apt install -y wget \
                    unzip \
					cmake
RUN apt-get install -y libssl1.1 \
			            libgstreamer1.0-0 \
			            gstreamer1.0-tools \
			            gstreamer1.0-plugins-good \
			            gstreamer1.0-plugins-bad \
			            gstreamer1.0-plugins-ugly \
			            gstreamer1.0-libav \
			            libgstreamer-plugins-base1.0-dev \
			            libgstrtspserver-1.0-0 \
			            libjansson4 \
			            libyaml-cpp-dev \
			            libjsoncpp-dev \
			            protobuf-compiler \
			            gcc \
			            g++ \
			            make \
			            build-essential \
			            python3 \
			            python3-pip
RUN apt-get install -y python3-gi \
			            python3-gst-1.0 \
			            python-gi-dev \
			            libglib2.0-dev \
			            python3-dev \
			            python3.8-dev \
			            libglib2.0-dev-bin \
			            libtool \
			            m4 \
			            autoconf \
			            automake
RUN apt install -y libboost-all-dev \
			        libfmt-dev \
			        libeigen3-dev \
			        libspdlog-dev
RUN mkdir -p /workspace
WORKDIR /workspace
# COPY build_opencv.sh /workspace/
# RUN chmod +x build_opencv.sh && ./build_opencv.sh
COPY requirements.txt /workspace/
RUN python3 -m pip install -r requirements.txt
WORKDIR /env
