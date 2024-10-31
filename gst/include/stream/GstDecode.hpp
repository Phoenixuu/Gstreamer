#ifndef GstDecode_hpp
#define GstDecode_hpp

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>

#include "common/BlockingQueue.hpp"
#include "Stream.hpp"
#include "define/Types.hpp"

class GstDecode : public Stream
{
    public:
        GstDecode(std::string strUrl);
        ~GstDecode();

        std::string getUrl() override;
        int getWidth() override;
        int getHeight() override;
        void getFrame(cv::Mat& mFrame) override;

        void runStream() override;
        void pauseStream() override;
        void stopStream() override;
        void join() override;

        void setOverlay(std::vector<stObject_t>& stObjects) override;
        void setOverlay(stObject_t& stObject) override;

    public:
        // GMainLoop* m_gstMainLoop;
        GstElement* m_gstPipeline;
        GstAppSink* m_gstAppSink;
        // GstElement* m_gstAppSink;
        GstBuffer* m_gstBuffer;
        GstMapInfo m_gstMapInfo;
        GstBus* m_gstBus;

    private:
        std::string getCodecStr(std::string strUrl);
        std::string genPileline(std::string strUrl);
        void makePipeline();

        bool gstBusProcessMsg(GstMessage *gstMsg);

        static void* gstBusFunc(void* arg);
        void runBusThread();

        static void* gstStreamFunc(void* arg);
        void runStreamThread();

    private:
        std::string m_strUrl;
        int m_iFrameWidth;
        int m_iFrameHeight;

        pthread_t m_pthread;
        pthread_t m_pthreadBus;
        pthread_mutex_t m_mtLock;

        bool m_bEnable = false;
        bool m_bRun = false;

        BlockingQueue<cv::Mat> m_quFrame;
        int m_iMaxQueueSize = 5;

};

#endif // GstDecode_hpp