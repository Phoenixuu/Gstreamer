#ifndef SCRFD_H
#define SCRFD_H

#include <vector>
#include <memory>
#include <string>

#include <opencv2/core/core.hpp>
#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEBuilder.hpp"
#include "SNPE/SNPEFactory.hpp"
#include "SnpeCommLib.hpp"
#include "DlSystem/ITensor.hpp"
#include "DlSystem/ITensorFactory.hpp"
#include "DlSystem/RuntimeList.hpp"
#include "DlSystem/StringList.hpp"
#include "DlContainer/IDlContainer.hpp"

#define input_width 640
#define input_height 640

enum { UNKNOWN,
       USERBUFFER_FLOAT,
       USERBUFFER_TF8,
       ITENSOR,
       USERBUFFER_TF16 };

struct FaceObject {
    cv::Rect_<float> rect;
    cv::Point2f point[5];
    float scores, vert_ratio_1, vert_ratio_2, eye_ratio_1, eye_ratio_2, nose_ratio;
    bool quality = true;
    std::string label;
};

typedef struct scrfd_params {
    std::unique_ptr<zdl::SNPE::SNPE> scrfd;
} scrfd_params;

class SCRFD {
   public:
    SCRFD();
    ~SCRFD();
    int load(std::string model_path, zdl::DlSystem::Runtime_t targetDevice);
    int execDetect(const cv::Mat& rgb, std::vector<FaceObject>& faceobjects, float scores_threshold = 0.5f, float nms_threshold = 0.45f);
    int draw(cv::Mat& rgb, const std::vector<FaceObject>& faceobjects);

   private:
    scrfd_params* s;
    bool has_kps = true;
};

#endif  // SCRFD_H
