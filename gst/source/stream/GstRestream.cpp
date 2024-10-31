#include "GstRestream.hpp"

#include "common/Logger.hpp"

GstRestream::GstRestream(int iIndex)
{
    m_iPort = 8000 + iIndex;

    // gst_init(nullptr, nullptr);

    if (pthread_mutex_init(&m_mtLock, NULL) != 0)
    {
        ERROR("mutex init failed!");
        return ;
    }

    gst_init(nullptr, nullptr);

    // std::string strPipe = "appsrc name=source is-live=true format=GST_FORMAT_TIME \
    //                         caps=video/x-raw,format=BGR,width=1920,height=1080,framerate=20/1 \
    //                         ! videoconvert ! x264enc speed-preset=superfast tune=zerolatency \
    //                         ! rtph264pay config-interval=-1 name=pay0 pt=96 \
    //                         ! udpsink host=127.0.0.1 port=" + std::to_string(m_iPort);

    // std::string strPipe = "appsrc name=source is-live=false format=GST_FORMAT_TIME \
    //                         ! videoconvert ! x264enc speed-preset=superfast tune=zerolatency \
    //                         ! rtph264pay config-interval=-1 name=pay0 pt=96 \
    //                         ! udpsink host=127.0.0.1 port=" + std::to_string(m_iPort);
                    
    // std::string strPipe = "appsrc name=source is-live=true format=GST_FORMAT_TIME \
    //                         ! videoconvert ! x265enc speed-preset=superfast tune=ssim \
    //                         ! rtph265pay config-interval=-1 name=pay0 pt=102 \
    //                         ! udpsink host=127.0.0.1 port=" + std::to_string(m_iPort);

    std::string strPipe = "appsrc name=source is-live=false format=GST_FORMAT_TIME \
                            ! videoconvert ! x264enc speed-preset=superfast tune=zerolatency slice-threads=true threads=2 \
                            ! h264parse \
                            ! queue ! flvmux streamable=true ! rtmpsink location=\"rtmp://172.24.108.140:1935/live/caimegicungdc\" sync=true";

    // std::string strPipe = "rtspsrc latency=0 location=\"rtsp://admin:Admin%40123@192.168.1.77/ONVIF/MediaInput?profile=def_profile1\" \
    //                         ! rtph264depay ! h264parse \
    //                         ! queue ! flvmux ! rtmpsink location=\"rtmp://172.24.108.140:1935/live/caimegicungdc\" sync=true";

    INFO("strPipe: {}", strPipe);

    m_gstPipeline = gst_parse_launch(strPipe.c_str(), nullptr);

    // m_gstAppSrc = gst_bin_get_by_name(GST_BIN(m_gstPipeline), "source");
    m_gstAppSrc = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(m_gstPipeline), "source"));

    pthread_create(&m_pthreadBus, NULL, gstBusFunc, this);
    pthread_detach(m_pthreadBus);

    pthread_create(&m_pthread, NULL, gstRestreamFunc, this);
    pthread_detach(m_pthread);

    // gst_element_set_state(m_gstPipeline, GST_STATE_PLAYING);

    // m_bEnable = true;
    // m_bRun = true;

}

GstRestream::~GstRestream()
{
    gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
    // gst_object_unref(m_gstPipeline);
    // gst_object_unref(m_gstBus);
    // gst_object_unref(m_gstAppSrc);

    // delete m_gstPipeline;
    // delete m_gstBus;
    // delete m_gstAppSrc;

    // pthread_join(m_pthreadBus, NULL);
    // pthread_join(m_pthread, NULL);

    pthread_cancel(m_pthreadBus);
    pthread_cancel(m_pthread);

    // gst_deinit();
}

void GstRestream::setFrame(cv::Mat& mImage)
{
    pthread_mutex_lock(&m_stContext->pthreadLock);
    m_stContext->mImage = mImage.clone();
    pthread_mutex_unlock(&m_stContext->pthreadLock);
}

void GstRestream::runRestream()
{
    pthread_mutex_lock(&m_mtLock);
    m_bRun = true;
    gst_element_set_state(m_gstPipeline, GST_STATE_PLAYING);
    pthread_mutex_unlock(&m_mtLock);
}

void GstRestream::pauseRestream()
{
    pthread_mutex_lock(&m_mtLock);
    m_bRun = false;
    gst_element_set_state(m_gstPipeline, GST_STATE_PAUSED);
    pthread_mutex_unlock(&m_mtLock);
}

void GstRestream::stopRestream()
{
    pthread_mutex_lock(&m_mtLock);
    m_bEnable = false;
    gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
    pthread_mutex_unlock(&m_mtLock);
}

void GstRestream::join()
{
    pthread_join(m_pthreadBus, NULL);
    pthread_join(m_pthread, NULL);
}

bool GstRestream::gstBusProcessMsg(GstMessage *gstMsg)
{
    GstMessageType gstMsgType = GST_MESSAGE_TYPE(gstMsg);

    switch (gstMsgType)
    {
        case GST_MESSAGE_ERROR:
        {
            GError *gstErr;
            gchar* g_dbg;
            gst_message_parse_error(gstMsg, &gstErr, &g_dbg);
            std::cout << "ERR = " << gstErr->message << " FROM " << GST_OBJECT_NAME(gstMsg->src) << std::endl;
            std::cout << "DBG = " << g_dbg << std::endl;
            g_clear_error(&gstErr);
            g_free(g_dbg);
            // exit(1);
            return false;
        }
        case GST_MESSAGE_EOS:
        {
            std::cout << "GST_MESSAGE_EOS" << std::endl;
            return false;
        }
        case GST_MESSAGE_STATE_CHANGED:
        {
            // std::cout << "GST_MESSAGE_STATE_CHANGED" << std::endl;
            if (GST_MESSAGE_SRC(gstMsg) == GST_OBJECT(m_gstPipeline))
            {
                GstState gstStateOld, gstStateNew, gstStatePenging;
                gst_message_parse_state_changed(gstMsg, &gstStateOld, &gstStateNew, &gstStatePenging);
                std::cout << "Pipeline changed from " 
                            << gst_element_state_get_name(gstStateOld) << " to " 
                            << gst_element_state_get_name(gstStateNew) << std::endl;
            }
            break;
        }
        case GST_MESSAGE_STEP_START:
        {
            // std::cout << "GST_MESSAGE_STEP_START" << std::endl;
            break;
        }
        case GST_MESSAGE_STREAM_STATUS:
        {
            // std::cout << "GST_MESSAGE_STREAM_STATUS" << std::endl;
            break;
        }
        case GST_MESSAGE_ELEMENT:
        {
            // std::cout << "GST_MESSAGE_ELEMENT" << std::endl;
            break;
        }
        default:
            break;
    }
    return true;
}

void* GstRestream::gstBusFunc(void* arg)
{
    GstRestream* pRestream = (GstRestream*)arg;
    pRestream->runBusThread();
    pthread_exit(NULL);
}

void GstRestream::runBusThread()
{
    m_gstBus = gst_element_get_bus(m_gstPipeline);

    bool bRes = false;
    while (true)
    {
        GstMessage *gstMsg = gst_bus_timed_pop(m_gstBus, GST_CLOCK_TIME_NONE);
        bRes = gstBusProcessMsg(gstMsg);
        gst_message_unref(gstMsg);
        if (bRes == false)
            break;
    }

    gst_object_unref(m_gstBus);
}

void GstRestream::NeedData(GstAppSrc* gstAppSrc, guint g_unused, gpointer g_data)
{
    // DEBUG("NeedData");
    stContext_t* stContext = (stContext_t*)g_data;

    cv::Mat mFrame;

    pthread_mutex_lock(&stContext->pthreadLock);
    mFrame = stContext->mImage.clone();
    stContext->mImage.release();
    pthread_mutex_unlock(&stContext->pthreadLock);

    if (mFrame.empty())
        mFrame = cv::Mat(stContext->iHeight, stContext->iWidth, CV_8UC3, cv::Scalar(114, 114, 114));

    // if (mFrame.empty())
    // {
    //     cv::Mat mYUV(stContext->iHeight + stContext->iHeight / 2, stContext->iWidth, CV_8UC1, cv::Scalar(114));
    //     // cv::cvtColor(mYUV, mFrame, cv::COLOR_YUV2BGR_I420);
    //     mFrame = mYUV;
    // }
    // cv::Mat mYUV;
    // cv::cvtColor(mFrame, mYUV, cv::COLOR_BGR2YUV_I420);

    memcpy(stContext->gstMapInfo.data, mFrame.data, gst_buffer_get_size(stContext->gstBuffer));

    GST_BUFFER_PTS(stContext->gstBuffer) = stContext->gstClockTime;
    GST_BUFFER_DURATION(stContext->gstBuffer) = gst_util_uint64_scale_int(1, GST_SECOND, stContext->iFPS);
    stContext->gstClockTime += GST_BUFFER_DURATION(stContext->gstBuffer);

    // GstFlowReturn gstFlowReturn = gst_app_src_push_buffer(gstAppSrc, stContext->gstBuffer);

    GstFlowReturn gstFlowReturn;
    g_signal_emit_by_name(gstAppSrc, "push-buffer", stContext->gstBuffer, &gstFlowReturn);
}

void* GstRestream::gstRestreamFunc(void* arg)
{
    GstRestream* pGstRestream = (GstRestream*)arg;
    pGstRestream->runRestreamThread();
    pthread_exit(NULL);
}

void GstRestream::runRestreamThread()
{
    // gst_init(nullptr, nullptr);

    // // std::string strPipe = "appsrc name=source is-live=true format=GST_FORMAT_TIME \
    // //                         caps=video/x-raw,format=BGR,width=1920,height=1080,framerate=20/1 \
    // //                         ! videoconvert ! x264enc speed-preset=superfast tune=zerolatency \
    // //                         ! rtph264pay config-interval=-1 name=pay0 pt=96 \
    // //                         ! udpsink host=127.0.0.1 port=" + std::to_string(m_iPort);

    // std::string strPipe = "appsrc name=source is-live=true format=GST_FORMAT_TIME \
    //                         ! videoconvert ! x264enc speed-preset=superfast tune=zerolatency \
    //                         ! rtph264pay config-interval=-1 name=pay0 pt=96 \
    //                         ! udpsink host=127.0.0.1 port=" + std::to_string(m_iPort);

    // m_gstPipeline = gst_parse_launch(strPipe.c_str(), nullptr);

    // // m_gstAppSrc = gst_bin_get_by_name(GST_BIN(m_gstPipeline), "source");
    // m_gstAppSrc = GST_APP_SRC(gst_bin_get_by_name(GST_BIN(m_gstPipeline), "source"));
    
    m_stContext = new stContext_t;
    m_stContext->iWidth = m_iWidth;
    m_stContext->iHeight = m_iHeight;
    m_stContext->iFPS = m_iFPS;

    std::string strCaps = "video/x-raw,format=BGR,width=" + std::to_string(m_stContext->iWidth) + ",height=" + std::to_string(m_stContext->iHeight) \
                            + ",framerate=" + std::to_string(m_stContext->iFPS) + "/1";

    // std::string strCaps = "video/x-flv,format=BGR,width=" + std::to_string(m_stContext->iWidth) + ",height=" + std::to_string(m_stContext->iHeight) \
    //                         + ",framerate=" + std::to_string(m_stContext->iFPS) + "/1";

    // std::string strCaps = "video/x-raw,format=I420,width=" + std::to_string(m_stContext->iWidth) + ",height=" + std::to_string(m_stContext->iHeight) \
    //                         + ",framerate=" + std::to_string(m_stContext->iFPS) + "/1";

    GstCaps* gstCaps = gst_caps_from_string(strCaps.c_str());
    g_object_set(m_gstAppSrc, "caps", gstCaps, nullptr);

    m_stContext->gstClockTime = 0;
    m_stContext->gstBuffer = gst_buffer_new_allocate(NULL, m_stContext->iHeight * m_stContext->iWidth * 3, NULL);
    // m_stContext->gstBuffer = gst_buffer_new_allocate(NULL, (m_stContext->iHeight + m_stContext->iHeight / 2) * m_stContext->iWidth, NULL);

    gst_buffer_map(m_stContext->gstBuffer, &m_stContext->gstMapInfo, GST_MAP_WRITE);

    g_signal_connect(m_gstAppSrc, "need-data", (GCallback)NeedData, m_stContext);

    gst_element_set_state(m_gstPipeline, GST_STATE_PLAYING);
}
