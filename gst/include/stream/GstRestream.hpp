#ifndef GstRestream_hpp
#define GstRestream_hpp

#include <iostream>
#include <string>
#include <pthread.h>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

typedef struct stContext
{
    GstClockTime gstClockTime;
    GstBuffer* gstBuffer;
    GstMapInfo gstMapInfo;
    cv::Mat mImage;
    int iWidth;
    int iHeight;
    int iFPS;
    pthread_mutex_t pthreadLock;
} stContext_t;

class GstRestream
{
    public:
        GstRestream(int iIndex);
        ~GstRestream();

        void setFrame(cv::Mat& mImage);

        void runRestream();
        void pauseRestream();
        void stopRestream();
        void join();

        static void NeedData(GstAppSrc* gstAppSrc, guint g_unused, gpointer g_data);

    public:
        GstElement* m_gstPipeline;
        GstBus* m_gstBus;
        GstAppSrc* m_gstAppSrc;
        // GstElement* m_gstAppSrc;

    private:
        bool gstBusProcessMsg(GstMessage *gstMsg);

        static void* gstBusFunc(void* arg);
        void runBusThread();

        static void* gstRestreamFunc(void* arg);
        void runRestreamThread();

    private:
        int m_iPort;
        int m_iWidth = 1280;
        int m_iHeight = 720;
        int m_iFPS = 10;

        pthread_t m_pthread;
        pthread_t m_pthreadBus;
        pthread_mutex_t m_mtLock;

        bool m_bEnable = false;
        bool m_bRun = false;

        stContext_t* m_stContext = nullptr;

};

#endif // GstRestream_hpp