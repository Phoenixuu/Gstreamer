#ifndef GstOverlay_hpp
#define GstOverlay_hpp

#include <iostream>
#include <string>
#include <pthread.h>
#include <unistd.h>

#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <gst/video/video.h>

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>
#include <freetype2/ft2build.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>

#include "common/BlockingQueue.hpp"
#include "Stream.hpp"
#include "define/Types.hpp"

typedef struct stOverlay
{
    gboolean gValid;
    GstVideoInfo gstVideoInfo;
    int iWidth;
    int iHeight;
    std::vector<stObject_t> stObjects;
    stObject_t stObject;
    pthread_mutex_t pthreadLock;
    // cairo_t* stCairo;
    cairo_font_face_t* stCairoFontFace;
} stOverlay_t;

class GstOverlay : public Stream
{
    public:
        GstOverlay(std::string strUrl, std::array<int, 4> arGrid);
        ~GstOverlay();

        std::string getUrl() override;
        int getWidth() override;
        int getHeight() override;
        void getFrame(cv::Mat& mFrame) override;

        void runStream() override;
        void pauseStream() override;
        void stopStream() override;
        void join() override;

        // std::string getUrl();
        // int getWidth();
        // int getHeight();
        // void getFrame(cv::Mat& mFrame);

        // void runStream();
        // void pauseStream();
        // void stopStream();
        // void join();

        void setOverlay(std::vector<stObject_t>& stObjects) override;
        void setOverlay(stObject_t& stObject) override;

        static void PrepareOverlay(GstElement* gstOverlay, GstCaps* gstCaps, gpointer g_data);
        // static void DrawOverlay(GstElement* gstOverlay, cairo_t* stCairo, guint64 g_timestamp, guint64 g_duration, gpointer g_data);
        static gboolean DrawOverlay(GstElement* gstOverlay, cairo_t* stCairo, guint64 g_timestamp, guint64 g_duration, gpointer g_data);

    public:
        // GMainLoop* m_gstMainLoop;
        GstElement* m_gstPipeline;
        GstAppSink* m_gstAppSink;
        // GstElement* m_gstAppSink;
        GstBuffer* m_gstBuffer;
        GstMapInfo m_gstMapInfo;
        GstElement* m_gstOverlay;
        GstBus* m_gstBus;

        // cairo_t* m_stCairo;
        // cairo_surface_t* m_stCairoSurface;
        // cairo_font_face_t* m_stCairoFontFace;

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
        std::array<int, 4> m_arGrid;
        int m_iFrameWidth;
        int m_iFrameHeight;

        pthread_t m_pthread;
        pthread_t m_pthreadBus;
        pthread_mutex_t m_mtLock;

        stOverlay_t* m_stOverlay = nullptr;

        bool m_bEnable = false;
        bool m_bRun = false;

        BlockingQueue<cv::Mat> m_quFrame;
        int m_iMaxQueueSize = 5;

};

#endif // GstOverlay_hpp