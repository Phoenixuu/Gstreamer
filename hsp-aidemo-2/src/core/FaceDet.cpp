#include "FaceDet.hpp"

#include <string.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

SCRFD::SCRFD() {
    this->s = new scrfd_params;
}

SCRFD::~SCRFD() {
    delete this->s;
}

static inline float intersection_area(const FaceObject &a, const FaceObject &b) {
    cv::Rect_<float> inter = a.rect & b.rect;
    return inter.area();
}

static void qsort_descent_inplace(std::vector<FaceObject> &faceobjects, int left, int right) {
    int i = left;
    int j = right;
    float p = faceobjects[(left + right) / 2].scores;

    while (i <= j) {
        while (faceobjects[i].scores > p)
            i++;

        while (faceobjects[j].scores < p)
            j--;

        if (i <= j) {
            // swap
            std::swap(faceobjects[i], faceobjects[j]);

            i++;
            j--;
        }
    }

    //     #pragma omp parallel sections
    {
        //         #pragma omp section
        {
            if (left < j) qsort_descent_inplace(faceobjects, left, j);
        }
        //         #pragma omp section
        {
            if (i < right) qsort_descent_inplace(faceobjects, i, right);
        }
    }
}

static void qsort_descent_inplace(std::vector<FaceObject> &faceobjects) {
    if (faceobjects.empty())
        return;

    qsort_descent_inplace(faceobjects, 0, faceobjects.size() - 1);
}

static void nms_sorted_bboxes(const std::vector<FaceObject> &faceobjects, std::vector<int> &picked, float nms_threshold) {
    picked.clear();

    const int n = faceobjects.size();

    std::vector<float> areas(n);
    for (int i = 0; i < n; i++) {
        areas[i] = faceobjects[i].rect.area();
    }

    for (int i = 0; i < n; i++) {
        const FaceObject &a = faceobjects[i];

        int keep = 1;
        for (int j = 0; j < (int)picked.size(); j++) {
            const FaceObject &b = faceobjects[picked[j]];

            // intersection over union
            float inter_area = intersection_area(a, b);
            float union_area = areas[i] + areas[picked[j]] - inter_area;
            //             float IoU = inter_area / union_area
            if (inter_area / union_area > nms_threshold)
                keep = 0;
        }

        if (keep)
            picked.push_back(i);
    }
}

static void generate_anchors(int base_size, int feat_stride, float anchors[][2]) {
    int row = 0;
    for (int i = 0; i < base_size; i++) {
        for (int j = 0; j < base_size; j++) {
            anchors[row][0] = j * feat_stride;
            anchors[row][1] = i * feat_stride;
            anchors[row + 1][0] = j * feat_stride;
            anchors[row + 1][1] = i * feat_stride;
            row += 2;
        }
    }
}

static void generate_proposals(float anchors[][2], int feat_stride, const float *oScore, const float *oBbox, const float *oKps,
                               size_t size, std::vector<FaceObject> &faceobjects, float scores_threshold) {
    for (size_t i = 0; i < size; i++) {
        float scores = static_cast<float>(oScore[i]);
        if (scores >= scores_threshold) {
            float x0 = anchors[i][0] - static_cast<float>(oBbox[4 * i] * feat_stride);
            float y0 = anchors[i][1] - static_cast<float>(oBbox[4 * i + 1] * feat_stride);
            float x1 = anchors[i][0] + static_cast<float>(oBbox[4 * i + 2] * feat_stride);
            float y1 = anchors[i][1] + static_cast<float>(oBbox[4 * i + 3] * feat_stride);

            x0 = std::max(std::min(x0, (float)input_width), 0.f);
            y0 = std::max(std::min(y0, (float)input_height), 0.f);
            x1 = std::max(std::min(x1, (float)input_width), 0.f);
            y1 = std::max(std::min(y1, (float)input_height), 0.f);

            float l_x0 = anchors[i][0] + static_cast<float>(oKps[10 * i] * feat_stride);
            float l_y0 = anchors[i][1] + static_cast<float>(oKps[10 * i + 1] * feat_stride);
            float l_x1 = anchors[i][0] + static_cast<float>(oKps[10 * i + 2] * feat_stride);
            float l_y1 = anchors[i][1] + static_cast<float>(oKps[10 * i + 3] * feat_stride);
            float l_x2 = anchors[i][0] + static_cast<float>(oKps[10 * i + 4] * feat_stride);
            float l_y2 = anchors[i][1] + static_cast<float>(oKps[10 * i + 5] * feat_stride);
            float l_x3 = anchors[i][0] + static_cast<float>(oKps[10 * i + 6] * feat_stride);
            float l_y3 = anchors[i][1] + static_cast<float>(oKps[10 * i + 7] * feat_stride);
            float l_x4 = anchors[i][0] + static_cast<float>(oKps[10 * i + 8] * feat_stride);
            float l_y4 = anchors[i][1] + static_cast<float>(oKps[10 * i + 9] * feat_stride);

            l_x0 = std::max(std::min(l_x0, (float)input_width), 0.f);
            l_y0 = std::max(std::min(l_y0, (float)input_height), 0.f);
            l_x1 = std::max(std::min(l_x1, (float)input_width), 0.f);
            l_y1 = std::max(std::min(l_y1, (float)input_height), 0.f);
            l_x2 = std::max(std::min(l_x2, (float)input_width), 0.f);
            l_y2 = std::max(std::min(l_y2, (float)input_height), 0.f);
            l_x3 = std::max(std::min(l_x3, (float)input_width), 0.f);
            l_y3 = std::max(std::min(l_y3, (float)input_height), 0.f);
            l_x4 = std::max(std::min(l_x4, (float)input_width), 0.f);
            l_y4 = std::max(std::min(l_y4, (float)input_height), 0.f);

            FaceObject obj;
            obj.scores = scores;
            obj.rect.x = x0;
            obj.rect.y = y0;
            obj.rect.width = x1 - x0;
            obj.rect.height = y1 - y0;
            obj.point[0].x = l_x0;
            obj.point[0].y = l_y0;
            obj.point[1].x = l_x1;
            obj.point[1].y = l_y1;
            obj.point[2].x = l_x2;
            obj.point[2].y = l_y2;
            obj.point[3].x = l_x3;
            obj.point[3].y = l_y3;
            obj.point[4].x = l_x4;
            obj.point[4].y = l_y4;
            faceobjects.push_back(obj);
        }
    }
}

int SCRFD::load(std::string containerPath, zdl::DlSystem::Runtime_t targetDevice) {
    std::unique_ptr<zdl::DlContainer::IDlContainer> container = loadContainerFromFile(containerPath);
    if (container == nullptr) {
        std::cout << "Load facedet model failed." << std::endl;
        return -1;
    }
    DlSystem::RuntimeList runtimeList(targetDevice);
    if (!zdl::SNPE::SNPEFactory::isRuntimeAvailable(targetDevice)) {
        std::cerr << "Selected runtime not present. Falling back to CPU." << std::endl;
        targetDevice = CPU_RUNTIME;
    }
    zdl::DlSystem::StringList outputs;
    // stride 8
    outputs.append("Sigmoid_157");
    outputs.append("Reshape_160");
    outputs.append("Reshape_163");
    // stride 16
    outputs.append("Sigmoid_182");
    outputs.append("Reshape_185");
    outputs.append("Reshape_188");
    // stride 32
    outputs.append("Sigmoid_207");
    outputs.append("Reshape_210");
    outputs.append("Reshape_213");
    int bufferType = ITENSOR;
    bool usingInitCaching = false;
    bool cpuFixedPointMode = false;
    bool useUserSuppliedBuffers = (bufferType == USERBUFFER_FLOAT || bufferType == USERBUFFER_TF8 || bufferType == USERBUFFER_TF16);
    zdl::DlSystem::PlatformConfig platformConfig;

    zdl::SNPE::SNPEBuilder snpeBuilder(container.get());
    this->s->scrfd = snpeBuilder.setOutputLayers(outputs)
                         .setRuntimeProcessorOrder(runtimeList)
                         .setUseUserSuppliedBuffers(useUserSuppliedBuffers)
                         .setPlatformConfig(platformConfig)
                         .setInitCacheMode(usingInitCaching)
                         .setCpuFixedPointMode(cpuFixedPointMode)
                         .build();
    if (this->s->scrfd == nullptr) {
        std::cerr << "Error while building facedet object." << std::endl;
        return -1;
    }
    std::cout << "Load facerec model successfully" << std::endl;
    return 0;
}

int SCRFD::execDetect(const cv::Mat &rgb, std::vector<FaceObject> &faceobjects, float scores_threshold, float nms_threshold) {
    std::unique_ptr<zdl::DlSystem::ITensor> input;
    input = convertMat2BgrFloat(this->s->scrfd, rgb);
    static zdl::DlSystem::TensorMap outputTensorMap;
    int exeStatus = this->s->scrfd->execute(input.get(), outputTensorMap);
    if (exeStatus == true) {
        // std::cout << "Execute SNPE Successfully" << std::endl;
    } else {
        std::cout << "Error while executing the network!" << std::endl;
    }
    zdl::DlSystem::StringList tensorNames = outputTensorMap.getTensorNames();
    std::vector<FaceObject> faceproposals;

    // stride 8
    {
        std::string scoreName = "443";
        std::string bboxName = "446";
        std::string kpsName = "449";

        zdl::DlSystem::ITensor *outTensorScore = outputTensorMap.getTensor(scoreName.c_str());
        zdl::DlSystem::ITensor *outTensorBbox = outputTensorMap.getTensor(bboxName.c_str());
        zdl::DlSystem::ITensor *outTensorKps = outputTensorMap.getTensor(kpsName.c_str());

        zdl::DlSystem::TensorShape scoreShape = outTensorScore->getShape();
        zdl::DlSystem::TensorShape bboxShape = outTensorBbox->getShape();
        zdl::DlSystem::TensorShape kpsShape = outTensorKps->getShape();

        const auto *oScore = reinterpret_cast<float *>(&(*outTensorScore->begin()));
        const auto *oBbox = reinterpret_cast<float *>(&(*outTensorBbox->begin()));
        const auto *oKps = reinterpret_cast<float *>(&(*outTensorKps->begin()));

        const int base_size = 80;
        // const int base_size = 48;
        const int feat_stride = 8;
        const int num_anchor = 2;
        const int cols_anchor = 2;
        const int rows_anchor = base_size * base_size * num_anchor;

        float anchors[rows_anchor][cols_anchor];
        generate_anchors(base_size, feat_stride, anchors);

        std::vector<FaceObject> faceobjects_32;
        generate_proposals(anchors, feat_stride, oScore, oBbox, oKps, scoreShape[0], faceobjects_32, scores_threshold);

        faceproposals.insert(faceproposals.end(), faceobjects_32.begin(), faceobjects_32.end());
    }

    // stride 16
    {
        std::string scoreName = "468";
        std::string bboxName = "471";
        std::string kpsName = "474";

        zdl::DlSystem::ITensor *outTensorScore = outputTensorMap.getTensor(scoreName.c_str());
        zdl::DlSystem::ITensor *outTensorBbox = outputTensorMap.getTensor(bboxName.c_str());
        zdl::DlSystem::ITensor *outTensorKps = outputTensorMap.getTensor(kpsName.c_str());

        zdl::DlSystem::TensorShape scoreShape = outTensorScore->getShape();
        zdl::DlSystem::TensorShape bboxShape = outTensorBbox->getShape();
        zdl::DlSystem::TensorShape kpsShape = outTensorKps->getShape();

        const auto *oScore = reinterpret_cast<float *>(&(*outTensorScore->begin()));
        const auto *oBbox = reinterpret_cast<float *>(&(*outTensorBbox->begin()));
        const auto *oKps = reinterpret_cast<float *>(&(*outTensorKps->begin()));

        const int base_size = 40;
        // const int base_size = 24;
        const int feat_stride = 16;
        const int num_anchor = 2;
        const int cols_anchor = 2;
        const int rows_anchor = base_size * base_size * num_anchor;

        float anchors[rows_anchor][cols_anchor];
        generate_anchors(base_size, feat_stride, anchors);

        std::vector<FaceObject> faceobjects_16;
        generate_proposals(anchors, feat_stride, oScore, oBbox, oKps, scoreShape[0], faceobjects_16, scores_threshold);

        faceproposals.insert(faceproposals.end(), faceobjects_16.begin(), faceobjects_16.end());
    }

    // stride 32
    {
        std::string scoreName = "493";
        std::string bboxName = "496";
        std::string kpsName = "499";

        zdl::DlSystem::ITensor *outTensorScore = outputTensorMap.getTensor(scoreName.c_str());
        zdl::DlSystem::ITensor *outTensorBbox = outputTensorMap.getTensor(bboxName.c_str());
        zdl::DlSystem::ITensor *outTensorKps = outputTensorMap.getTensor(kpsName.c_str());

        zdl::DlSystem::TensorShape scoreShape = outTensorScore->getShape();
        zdl::DlSystem::TensorShape bboxShape = outTensorBbox->getShape();
        zdl::DlSystem::TensorShape kpsShape = outTensorKps->getShape();

        const auto *oScore = reinterpret_cast<float *>(&(*outTensorScore->begin()));
        const auto *oBbox = reinterpret_cast<float *>(&(*outTensorBbox->begin()));
        const auto *oKps = reinterpret_cast<float *>(&(*outTensorKps->begin()));

        const int base_size = 20;
        // const int base_size = 12;
        const int feat_stride = 32;
        const int num_anchor = 2;
        const int cols_anchor = 2;
        const int rows_anchor = base_size * base_size * num_anchor;

        float anchors[rows_anchor][cols_anchor];
        generate_anchors(base_size, feat_stride, anchors);

        std::vector<FaceObject> faceobjects_8;
        generate_proposals(anchors, feat_stride, oScore, oBbox, oKps, scoreShape[0], faceobjects_8, scores_threshold);

        faceproposals.insert(faceproposals.end(), faceobjects_8.begin(), faceobjects_8.end());
    }

    float width = rgb.cols;
    float height = rgb.rows;
    qsort_descent_inplace(faceproposals);

    std::vector<int> picked;
    nms_sorted_bboxes(faceproposals, picked, nms_threshold);

    int face_count = picked.size();

    faceobjects.resize(face_count);
    for (int i = 0; i < face_count; i++) {
        faceobjects[i] = faceproposals[picked[i]];

        float x = faceobjects[i].rect.x;
        float y = faceobjects[i].rect.y;
        float w = faceobjects[i].rect.width;
        float h = faceobjects[i].rect.height;

        faceobjects[i].rect.x = x / (float)input_width * width;
        faceobjects[i].rect.y = y / (float)input_height * height;
        faceobjects[i].rect.width = w / (float)input_width * width;
        faceobjects[i].rect.height = h / (float)input_width * height;

        float x0 = faceobjects[i].point[0].x;
        float y0 = faceobjects[i].point[0].y;
        float x1 = faceobjects[i].point[1].x;
        float y1 = faceobjects[i].point[1].y;
        float x2 = faceobjects[i].point[2].x;
        float y2 = faceobjects[i].point[2].y;
        float x3 = faceobjects[i].point[3].x;
        float y3 = faceobjects[i].point[3].y;
        float x4 = faceobjects[i].point[4].x;
        float y4 = faceobjects[i].point[4].y;

        faceobjects[i].point[0].x = x0 / (float)input_width * width;
        faceobjects[i].point[0].y = y0 / (float)input_height * height;
        faceobjects[i].point[1].x = x1 / (float)input_width * width;
        faceobjects[i].point[1].y = y1 / (float)input_height * height;
        faceobjects[i].point[2].x = x2 / (float)input_width * width;
        faceobjects[i].point[2].y = y2 / (float)input_height * height;
        faceobjects[i].point[3].x = x3 / (float)input_width * width;
        faceobjects[i].point[3].y = y3 / (float)input_height * height;
        faceobjects[i].point[4].x = x4 / (float)input_width * width;
        faceobjects[i].point[4].y = y4 / (float)input_height * height;
    }
    return 0;
}

int SCRFD::draw(cv::Mat &rgb, const std::vector<FaceObject> &faceobjects) {
    for (size_t i = 0; i < faceobjects.size(); i++) {
        const FaceObject &obj = faceobjects[i];

        cv::rectangle(rgb, obj.rect, cv::Scalar(0, 0, 255), 2);

        if (has_kps) {
            cv::circle(rgb, obj.point[0], 1, cv::Scalar(255, 255, 0), -1);
            cv::circle(rgb, obj.point[1], 1, cv::Scalar(255, 255, 0), -1);
            cv::circle(rgb, obj.point[2], 1, cv::Scalar(255, 255, 0), -1);
            cv::circle(rgb, obj.point[3], 1, cv::Scalar(255, 255, 0), -1);
            cv::circle(rgb, obj.point[4], 1, cv::Scalar(255, 255, 0), -1);
        }

        char text[256];
        sprintf(text, "%.1f%%", obj.scores * 100);

        int baseLine = 0;
        cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.8, 2, &baseLine);

        int x = obj.rect.x;
        int y = obj.rect.y - label_size.height - baseLine;
        if (y < 0)
            y = 0;
        if (x + label_size.width > rgb.cols)
            x = rgb.cols - label_size.width;
    }
    return 0;
}
