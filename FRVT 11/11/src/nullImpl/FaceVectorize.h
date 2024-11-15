//
// Created by maicg on 3/25/23.
//
#ifndef FACEVECTORIZE_H
#define FACEVECTORIZE_H

#include <cmath>
#include <iostream>
#include <string>
#include "net.h"
#include "opencv2/opencv.hpp"

class FaceVectorize {
    public:
        int load(const std::string &param_file, const std::string &weight_file, bool use_gpu);
        
        std::vector<std::vector<float>> infer_FaceVectorize(cv::Mat& img);

        void normalize(std::vector<float> &feature);
    private:
        ncnn::Net net_FaceVectorize;
        
};

double cosine_similarity(std::vector<float> v1, std::vector<float> v2);

#endif //FACEVECTORIZE_H