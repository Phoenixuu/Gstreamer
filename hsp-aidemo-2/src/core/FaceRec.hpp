#ifndef SNPEINSIGHTFACE_H
#define SNPEINSIGHTFACE_H

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>

#include "DlSystem/DlError.hpp"
#include "DlSystem/RuntimeList.hpp"
#include "DlSystem/UserBufferMap.hpp"
#include "DlSystem/IUserBuffer.hpp"
#include "DlContainer/IDlContainer.hpp"
#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEBuilder.hpp"
#include "DiagLog/IDiagLog.hpp"
#include "SNPE/SNPEFactory.hpp"
#include "DlSystem/DlVersion.hpp"
#include "DlSystem/DlEnums.hpp"
#include "DlSystem/String.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include "DlSystem/PlatformConfig.hpp"

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

struct class_info {
    double min_distance;
    int index;
};

class SnpeInsightface {
   private:
    std::unique_ptr<zdl::SNPE::SNPE> snpeInsightface;

   public:
    SnpeInsightface(/* args */);
    ~SnpeInsightface();
    int load(std::string containerPath, zdl::DlSystem::Runtime_t targetDevice);
    cv::Mat execRecog(const cv::Mat& img);
    class_info classify(const cv::Mat& img, const cv::Mat& cmp);
};

#endif
