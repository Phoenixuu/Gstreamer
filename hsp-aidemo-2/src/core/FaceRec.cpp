#include "FaceRec.hpp"
#include "SnpeCommLib.hpp"

SnpeInsightface::SnpeInsightface(/* args */) {
    this->snpeInsightface = std::unique_ptr<zdl::SNPE::SNPE>();
}

SnpeInsightface::~SnpeInsightface() {
    this->snpeInsightface.release();
}

int SnpeInsightface::load(std::string containerPath, zdl::DlSystem::Runtime_t targetDevice) {
    std::unique_ptr<zdl::DlContainer::IDlContainer> container = loadContainerFromFile(containerPath);
    if (container == nullptr) {
        std::cerr << "Load facerec model failed." << std::endl;
        return -1;
    }
    DlSystem::RuntimeList runtimeList(targetDevice);
    if (!zdl::SNPE::SNPEFactory::isRuntimeAvailable(targetDevice)) {
        std::cerr << "Selected runtime not present. Falling back to CPU." << std::endl;
        targetDevice = CPU_RUNTIME;
    }
    zdl::DlSystem::StringList outputs;
    this->snpeInsightface = setBuilderOptions(container, runtimeList, outputs);
    if (this->snpeInsightface == nullptr) {
        std::cerr << "Error while building facerec object." << std::endl;
        return -1;
    }
    std::cout << "Load facedet model successfully" << std::endl;
    return 0;
}

cv::Mat SnpeInsightface::execRecog(const cv::Mat& img) {
    std::unique_ptr<zdl::DlSystem::ITensor> input;
    input = convertMat2BgrFloat(this->snpeInsightface, img);
    static zdl::DlSystem::TensorMap outputTensorMap;
    int exeStatus = this->snpeInsightface->execute(input.get(), outputTensorMap);
    if (exeStatus == true) {
        // printf("Execute SNPE successfully \n");
    } else {
        printf("Error while executing the network \n");
    }
    zdl::DlSystem::StringList tensorNames = outputTensorMap.getTensorNames();
    zdl::DlSystem::ITensor* outTensor = outputTensorMap.getTensor(tensorNames.at(0));
    float* outData = reinterpret_cast<float*>(&(*outTensor->begin()));
    zdl::DlSystem::TensorShape outShape = outTensor->getShape();
    std::vector<float> feature(outShape[1]);
    feature.clear();
    for (size_t t = 0; t < outShape[1]; t++) {
        auto output = static_cast<float>(outData[t]);
        feature.push_back(output);
    }
    cv::Mat outMat = cv::Mat(feature, true).reshape(1, 1);
    cv::normalize(outMat, outMat);
    return outMat;
}

class_info SnpeInsightface::classify(const cv::Mat& img, const cv::Mat& cmp) {
    int rows = cmp.rows;
    cv::Mat broad;
    cv::repeat(img, rows, 1, broad);
    broad = broad - cmp;
    cv::pow(broad, 2, broad);
    cv::reduce(broad, broad, 1, cv::REDUCE_SUM);
    double dis;
    cv::Point point;
    cv::minMaxLoc(broad, &dis, 0, &point, 0);
    return class_info{dis, point.y};
}