#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <fstream>

#include "/home/dunggps/folder/gst/include/common/Logger.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "StreamManager.hpp"
// #include "GstRestream.hpp"

void testStream()
{
    std::string strUrl = "../data/videos/vid01.mp4";
    // std::string strUrl = "rtsp://admin:abcd1234@192.168.1.109";

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

        cv::resize(mFrame, mFrame, cv::Size(1280, 720));

        cv::imshow("stream", mFrame);
        cv::waitKey(1);
    }
}

void testMultistream()
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

    // iNumChannel = 1;
    for (int iIdx = 0; iIdx < iNumChannel; iIdx++)
    {
        std::thread streamThread([strUrls, iIdx] ()
            {
                std::string strUrl = strUrls[iIdx];

                std::shared_ptr<Stream> shpStream = StreamManager::GetInstance().getStream(strUrl);

                while (true)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(5));

                    cv::Mat mFrame;
                    shpStream->getFrame(mFrame);

                    // INFO("mFrame.size() = {} x {}", mFrame.cols, mFrame.rows);
                    if (mFrame.empty())
                        continue;

                    cv::resize(mFrame, mFrame, cv::Size(1280, 720));

                    cv::imshow("stream " + std::to_string(iIdx), mFrame);
                    cv::waitKey(1);
                }
            }
        );
        streamThread.detach();
    }
}

// void testRestream()
// {
//     std::string strUrl = "../data/videos/vid01.mp4";

//     std::shared_ptr<Stream> shpStream = StreamManager::GetInstance().getStream(strUrl);

//     GstRestream* pGstRestream = new GstRestream(0);

//     while (true)
//     {
//         std::this_thread::sleep_for(std::chrono::milliseconds(5));

//         cv::Mat mFrame;
//         shpStream->getFrame(mFrame);

//         // INFO("mFrame.size() = {} x {}", mFrame.cols, mFrame.rows);
//         if (mFrame.empty())
//             continue;

//         cv::resize(mFrame, mFrame, cv::Size(1920, 1080));

//         pGstRestream->setFrame(mFrame);
//     }
// }

int main()
{
    INFO("===== START =====");
    
    // testStream();
    testMultistream();
    // testRestream();

    while (true)
    {
        ;
    }

    return 0;
}