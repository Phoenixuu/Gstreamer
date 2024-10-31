#include <iostream>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "common/Logger.hpp"
#include "StreamManager.hpp"
#include "AIModel.hpp"
#include "GstOverlay.hpp"

void test()
{
    std::string strUrl = "../data/videos/vid01.mp4";
    // std::string strUrl = "rtsp://admin:abcd1234@192.168.1.109";

    std::ifstream fsJsonFile("../data/json/yolov8.json");
    nlohmann::json jConfigModel = nlohmann::json::parse(fsJsonFile);

    AIModel* m_pYoloNAS = new AIModel(jConfigModel, JN_MODEL_YOLOV8);

    std::shared_ptr<Stream> shpStream = StreamManager::GetInstance().getStream(strUrl);
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        cv::Mat mFrame;
        shpStream->getFrame(mFrame);

        // INFO("mFrame.size() = {} x {}", mFrame.cols, mFrame.rows);
        if (mFrame.empty())
            continue;

        std::vector<stObject_t> stOutputs;
        m_pYoloNAS->run(mFrame, stOutputs);

        cv::Mat mOutput = mFrame.clone();
        for (auto stOut : stOutputs)
        {
            // std::cout << "id: " << stOut.strLabel << ", score: " << stOut.fScore << ", " << stOut.rfBox << std::endl;
            cv::rectangle(mOutput, stOut.rfBox, cv::Scalar(0, 255, 0), 1, 8);
            cv::putText(mOutput, stOut.strLabel, stOut.rfBox.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2, 8);
        }

        cv::resize(mOutput, mOutput, cv::Size(1280, 720));

        cv::imshow("stream", mOutput);
        cv::waitKey(1);
    }
}

// void testAIRestream()
// {
//     std::string strUrl = "../data/videos/vid01.mp4";
//     // std::string strUrl = "rtsp://admin:abcd1234@192.168.1.109";

//     // std::ifstream fsJsonFile("../data/json/yolonas.json");
//     std::ifstream fsJsonFile("../data/json/yolov8.json");
//     nlohmann::json jConfigModel = nlohmann::json::parse(fsJsonFile);

//     // AIModel* m_pYoloNAS = new AIModel(jConfigModel, JN_MODEL_YOLONAS);
//     AIModel* m_pYoloV8 = new AIModel(jConfigModel, JN_MODEL_YOLOV8);

//     std::shared_ptr<Stream> shpStream = StreamManager::GetInstance().getStream(strUrl);
//     // std::this_thread::sleep_for(std::chrono::milliseconds(5000));

//     GstRestream* pGstRestream = new GstRestream(0);

//     while (true)
//     {
//         std::this_thread::sleep_for(std::chrono::milliseconds(5));

//         cv::Mat mFrame;
//         shpStream->getFrame(mFrame);

//         // INFO("mFrame.size() = {} x {}", mFrame.cols, mFrame.rows);
//         if (mFrame.empty())
//             continue;

//         std::vector<stObject_t> stOutputs;
//         // m_pYoloNAS->run(mFrame, stOutputs);
//         m_pYoloV8->run(mFrame, stOutputs);

//         cv::Mat mOutput = mFrame.clone();
//         for (auto stOut : stOutputs)
//         {
//             // std::cout << "id: " << stOut.strLabel << ", score: " << stOut.fScore << ", " << stOut.rfBox << std::endl;
//             cv::rectangle(mOutput, stOut.rfBox, cv::Scalar(0, 255, 0), 1, 8);
//             cv::putText(mOutput, stOut.strLabel, stOut.rfBox.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2, 8);
//         }

//         cv::resize(mOutput, mOutput, cv::Size(1280, 720));

//         pGstRestream->setFrame(mOutput);
//     }
// }

void testOverlay()
{
    std::string strUrl = "../data/videos/vid01.mp4";
    // std::string strUrl = "rtsp://admin:abcd1234@192.168.1.109";

    std::ifstream fsJsonFile("../data/json/yolov8.json");
    nlohmann::json jConfigModel = nlohmann::json::parse(fsJsonFile);

    AIModel* m_pYoloV8 = new AIModel(jConfigModel, JN_MODEL_YOLOV8);

    // GstOverlay* pStream = new GstOverlay(strUrl);
    std::shared_ptr<Stream> shpStream = StreamManager::GetInstance().getStream(strUrl);
    // std::this_thread::sleep_for(std::chrono::milliseconds(5000));

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));

        cv::Mat mFrame;
        shpStream->getFrame(mFrame);

        // INFO("mFrame.size() = {} x {}", mFrame.cols, mFrame.rows);
        if (mFrame.empty())
            continue;

        std::vector<stObject_t> stOutputs;
        m_pYoloV8->run(mFrame, stOutputs);
        // INFO("stOutputs.size() = {}", stOutputs.size());

        shpStream->setOverlay(stOutputs);

        // cv::Mat mOutput = mFrame.clone();
        // for (auto stOut : stOutputs)
        // {
        //     // std::cout << "id: " << stOut.strLabel << ", score: " << stOut.fScore << ", " << stOut.rfBox << std::endl;
        //     // cv::rectangle(mOutput, stOut.rfBox, cv::Scalar(0, 255, 0), 1, 8);
        //     // cv::putText(mOutput, stOut.strLabel, stOut.rfBox.tl(), cv::FONT_HERSHEY_SIMPLEX, 1.0, cv::Scalar(0, 255, 0), 2, 8);
        //     shpStream->setOverlay(stOut);
        // }

        // cv::resize(mOutput, mOutput, cv::Size(1280, 720));

        // cv::imshow("stream", mOutput);
        // cv::waitKey(1);
    }
}

void testMultiOverlay()
{
    std::string strConfigPath = "../data/config";
    int iNumChannel = 0;

    std::vector<std::string> strUrls;
    std::string strLine;

    std::ifstream fsConfigFile(strConfigPath);

    if (fsConfigFile.is_open())
    {
        while (getline(fsConfigFile, strLine))
        {
            INFO("channel {} -- {}", iNumChannel, strLine);
            strUrls.emplace_back(strLine);
            DEBUG(strUrls[iNumChannel]);
            iNumChannel += 1;
        }
        fsConfigFile.close();
    }
    else {
        ERROR("Unable to open config file: {}", strConfigPath);
        return ;
    }

    iNumChannel = 1;
    for (int iIdx = 0; iIdx < iNumChannel; iIdx++)
    {
        std::thread streamThread([strUrls, iIdx] ()
            {
                std::ifstream fsJsonFile("../data/json/yolov8.json");
                nlohmann::json jConfigModel = nlohmann::json::parse(fsJsonFile);

                AIModel* m_pYoloV8 = new AIModel(jConfigModel, JN_MODEL_YOLOV8);

                std::string strUrl = strUrls[iIdx];
                // GstOverlay* pStream = new GstOverlay(strUrl);
                std::shared_ptr<Stream> shpStream = StreamManager::GetInstance().getStream(strUrl);

                while (true)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));

                    cv::Mat mFrame;
                    shpStream->getFrame(mFrame);

                    // INFO("mFrame.size() = {} x {}", mFrame.cols, mFrame.rows);
                    if (mFrame.empty())
                        continue;

                    std::vector<stObject_t> stOutputs;
                    m_pYoloV8->run(mFrame, stOutputs);
                    // INFO("stOutputs.size() = {}", stOutputs.size());

                    shpStream->setOverlay(stOutputs);
                }
            }
        );
        streamThread.detach();
    }
}

int main()
{
    INFO("===== START =====");
    
    // test();
    // testAIRestream();
    // testOverlay();
    testMultiOverlay();

    while (true)
    {
        ;
    }

    return 0;
}