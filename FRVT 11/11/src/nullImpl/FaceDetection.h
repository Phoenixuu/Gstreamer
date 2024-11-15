// NCS Group is the owner of this software. All rights reserved.
//
// Copyright (C) 2023 NCS Group. All rights reserved.
//
// This software and its associated documentation are confidential and proprietary
// information of NCS Group. Unauthorized use, reproduction or distribution of this
// software or any portion of it is strictly prohibited.
//
// This software is provided "AS IS" without warranty of any kind, either express or implied,
// including but not limited to the implied warranties of merchantability and fitness for a
// particular purpose.

/**
* Created by hiep.tran@ncsgroup.vn on 14/03/2023.
**/

#ifndef FACEDETECTION_H
#define FACEDETECTION_H

#include <opencv2/core/core.hpp>

#include <net.h>

struct FaceObject
{
    cv::Rect_<float> rect;
    cv::Point2f landmark[5];
    float prob;
};

class FaceDetection
{
public:
    // int load(const char* modeltype, bool use_gpu = false);

    // int load(AAssetManager* mgr, const char* modeltype, bool use_gpu = false);
    int load(const std::string &param_file, const std::string &weight_file, bool use_gpu);

    int detect(cv::Mat& rgb, std::vector<FaceObject>& faceobjects, float prob_threshold = 0.15f, float nms_threshold = 0.45f);

    int draw(cv::Mat& rgb, FaceObject& obj, std::string &text);

    std::vector<bool> process_kps(FaceObject& obj, float& area_base);

private:
    ncnn::Net net_FaceDetection;
    bool has_kps;
};

cv::Mat alignment(cv::Mat& img, FaceObject& faceobjects);

#endif // FACEDETECTION_H