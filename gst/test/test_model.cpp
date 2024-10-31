#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <chrono>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "common/Logger.hpp"

#include "AIModel.hpp"

void testYoloV8()
{
    std::ifstream fsJsonFile("../data/json/yolov8.json");
    nlohmann::json jConfigModel = nlohmann::json::parse(fsJsonFile);

    AIModel* m_pYoloV8 = new AIModel(jConfigModel, JN_MODEL_YOLOV8);

    cv::Mat mImage = cv::imread("../data/images/40.jpg", cv::IMREAD_COLOR);

    std::vector<stObject_t> stOutputs;
    m_pYoloV8->run(mImage, stOutputs);

    INFO("stOutputs.size() = {}", stOutputs.size());

    cv::Mat mOutput = mImage.clone();
    for (auto stOut : stOutputs)
    {
        std::cout << "id: " << stOut.strLabel << ", score: " << stOut.fScore << ", " << stOut.rfBox << std::endl;
        cv::rectangle(mOutput, stOut.rfBox, cv::Scalar(0, 255, 0), 1, 8);
        cv::putText(mOutput, stOut.strLabel, stOut.rfBox.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2, 8);
    }

    cv::resize(mOutput, mOutput, cv::Size(1280, 720));

    cv::imshow("mOutput", mOutput);
    cv::waitKey(0);
}

int main()
{
    INFO("===== START =====");
    
    testYoloV8();

    while (true)
    {
        ;
    }

    return 0;
}