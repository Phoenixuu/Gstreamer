//
// Created by hieptran on 3/25/23.
//

#ifndef CERBERUSAISDK_FACEWARP_H
#define CERBERUSAISDK_FACEWARP_H

#include "opencv2/opencv.hpp"
#include "FaceDetection.h"

class FaceWarp {
public:
    FaceWarp();
    virtual ~FaceWarp();
    double angle;
    cv::Mat Process(cv::Mat &SmallFrame, FaceObject& Obj);
private:
    static int     MatrixRank(cv::Mat M);
    static cv::Mat VarAxis0(const cv::Mat &src);
    static cv::Mat MeanAxis0(const cv::Mat &src);
    static cv::Mat ElementwiseMinus(const cv::Mat &A,const cv::Mat &B);
    static cv::Mat SimilarTransform(cv::Mat src,cv::Mat dst);
};

#endif //CERBERUSAISDK_FACEWARP_H
