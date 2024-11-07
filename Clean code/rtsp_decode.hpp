#ifndef VIDEODECODER_HPP
#define VIDEODECODER_HPP

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>

#include "Stream.hpp"

class RtspDecode : public Stream
{
public:
    RtspDecode(std::string strUrl);
    ~RtspDecode();

    std::string getUrl() override;
    int getWidth() override;
    int getHeigh() override;
    void getFrame(cv::Mat& mFrame) override;

    void runStream() override;
    void pauseStream() override;
    void stopStream() override;
    void join() override;

public:
    GMainLoop* m_gstMainLoop;
    GstElement* m_gstPipeline;
    GstAppsink* m_gstAppsink;
    GstBuffer* m_gstBuffer;
    GstMapInfo m_gstMapInfo;
    GstBus* m_gstBus;

private:
    std::string getCodeStr(std::string strUrl);
    void makepipeline()

    bool gstBusProcessMsg(GstMesseage *gstMsg);

    static void* getBusFunc(void* arg);
    void runBusThread();

    static void* gstStreamFunc(void* arg);
    void runStreamThread();

private:
    std::string m_strUrl;
    int m_iFrameWidth;
    int m_iFrameHeigh;

    pthread_t m_pthread;
    pthread_t m_pthreadBus;
    pthread_t m_pthreadGetFrame;
    pthread_mutex_t m_mtLock;

    bool m_bEnable = false;
    bool m_bRun = false;    

    BlockingQueue<cv::Mat> m_quFrame;
    int m_MaxQueueSize = 5;
};

// class MainLoopManager {
// public:
//     static GMainLoop* getInstance();
// };

// class GstElementFactory {
// public:
//     static GstElement* createElement(const std::string& factory_name, const std::string& element_name);
// };

// class VideoDecoder {
// public:
//     explicit VideoDecoder(const std::string& rtsp_uri);
//     ~VideoDecoder();

// private:
//     GstElement* pipeline = nullptr;
//     int count;

//     void setupPipeline(const std::string& rtsp_uri);
//     bool linkTeeToQueues(GstElement* tee, GstElement* queue1, GstElement* queue2, GstElement* sink, GstElement* videosink);

//     static GstFlowReturn on_new_sample(GstElement* sink, VideoDecoder* decoder);
//     static void on_pad_added(GstElement* src, GstPad* new_pad, gpointer user_data);
// };

#endif // VIDEODECODER_HPP
