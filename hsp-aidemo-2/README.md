* Build and run demo on host side:
  
  sudo apt-get update
  
  sudo apt-get install libgstreamer1.0-0 gstreamer1.0-plugins-base gstreamer1.0-plugins-good gstreamer1.0-plugins-bad gstreamer1.0-plugins-ugly gstreamer1.0-libav libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
  
  sudo apt install clang
  
  sudo apt-get install libc++1 libc++abi1
  
  sudo apt install cmake
  
  git clone https://github.com/nguyennhusonars/hsp-aidemo
  
  cd <path-to-project>
  
  mkdir build && cd build
  
  cmake -DBUILD_X86=1 ..
  
  make -j4

* Build and run demo on device side:
  
  apt-get update  

  apt install libcairo2-dev
  
  apt install cmake
  
  cmake -DBUILD_AARCH64=1 ..
  
  make -j4

* Prepare .dlc models:

cd ${SNPE_ROOT}/examples/Models/

mkdir scrfd && cd scrfd

Download SCRFD model from: https://github.com/deepinsight/insightface/tree/master/python-package

Push the model to ${SNPE_ROOT}/examples/Models/scrfd/onnx/

Convert from onnx to dlc: snpe-onnx-to-dlc --input_network onnx/buffalo_s/det_500m.onnx --input_dim input.1 "1,3,640,640" --output_path $SNPE_ROOT/examples/Models/scrfd/dlc/det_500m.dlc

Prepare raw image list:

mkdir -p data/cropped

python scripts/create_scrfd_raws.py -i data/ -d data/cropped/

python scripts/create_file_list.py -i data/cropped/ -o data/cropped/raw_list.txt -e *.raw

python scripts/create_file_list.py -i data/cropped/ -o data/target_raw_list.txt -e *.raw -r

Quantize the dlc model: snpe-dlc-quantize --input_dlc dlc/det_500m.dlc --input_list data/cropped/raw_list.txt --output_dlc dlc/det_500m_quantized.dlc
