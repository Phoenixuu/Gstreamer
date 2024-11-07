#include "rtsp_decode.hpp"

RtspDecode::RtspDecode(std::string strUrl)
{
    m_strUrl = strUrl;

    gst_init(nullptr, nullptr);

    if(pthread_mutex_init(&m_mtLock, NULL) != 0)
    {
        ERROR("mutex init failed!");
        return;
    }

    makePipeline();

    pthread_create(&m_pthreadBus, NULL, gstBusFunc, this);
    pthread_detach(m_pthreadBus);

    pthread_create(&m_pthread, NULL, gstBusFunc, this)
    pthread_detach(m_pthread);

    m_bEnable = true;
    m_bRun = true;
}

RtspDecode::~RtspDecode(){
    gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
    gst_object_unref(m_gstPipeline);
    gst_object_unref(m_gstBus);
    gst_object_unref(m_gstAppSink);

    gst_deinit();

    pthread_cancel(m_pthreadBus);
    pthread_cancel(m_pthread);


    while (m_quFrame.getSize() > 0)
    {
        cv::Mat mFrame;
        m_quFrame.tryWaitAndPop(mFrame, 50);
    }
}

std::string GstDecode::getUrl()
{
    return m_strUrl;
}

int GstDecode::getWidth()
{
    return m_iFrameWidth;
}

int GstDecode::getHeight()
{
    return m_iFrameHeight;
}

void GstDecode::getFrame(cv::Mat& mFrame)
{
    pthread_mutex_lock(&m_mtLock);
    if(!m_quFrame.empty()){
        mFrame = m_quFrame.front();
        m_quFrame.pop();
    }
    pthread_mutex_unlock(&m_mtLock);
}

void GstDecode::runStream()
{
    pthread_mutex_lock(&m_mtLock);
    m_bRun = true;
    pthread_mutex_unlock(&m_mtLock);
}

void GstDecode::pauseStream()
{
    pthread_mutex_lock(&m_mtLock);
    m_bRun = false;
    pthread_mutex_unlock(&m_mtLock);
}

void GstDecode::stopStream()
{
    pthread_mutex_lock(&m_mtLock);
    m_bEnable = false;
    pthread_mutex_unlock(&m_mtLock);
}

void GstDecode::join()
{
    pthread_join(m_pthreadBus, NULL);
    pthread_join(m_pthread, NULL);
}

std::string GstDecode::getCodecStr(std::string strUrl)
{
    cv::VideoCapture videoCapture;
    videoCapture.open(strUrl);

    if (videoCapture.isOpened() == false)
        return "";

    m_iFrameWidth = videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
    m_iFrameHeight = videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);

    double dFourcc = videoCapture.get(cv::CAP_PROP_FOURCC);
    const char* pCodec;
    int iFourcc = static_cast<int>(dFourcc);
    if (iFourcc == 0)
        pCodec = nullptr;
    
    static char arCodec[5];
    for (int i=0; i < 4; i++)
    {
        arCodec[i] = static_cast<char>((iFourcc >> (8 * i)) & 0x0FF);
    }
    arCodec[4] = '\0';
    pCodec = arCodec;

    videoCapture.release();

    std::string strCodec(pCodec);

    return strCodec;
}

std::string GstDecode::genPileline(std::string strUrl)
{
    return strPipeline;
}


void GstDecode::makePipeline()
{
}


bool GstDecode::gstBusProcessMsg(GstMessage *gstMsg)
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

void* GstDecode::gstGetFrameFunc(void* arg){
    GstDecode* pStream = (GstDecode*)arg;

    while (pStream->m_bEnable) {
        cv::Mat frame;
        pStream->getGstFrame(frame);

        if (!frame.empty()) {
            pthread_mutex_lock(&pStream->m_mtLock);
            if (pStream->m_quFrame.size() < pStream->m_iMaxQueueSize) {
                pStream->m_quFrame.push(frame); 
            }
            pthread_mutex_unlock(&pStream->m_mtLock);
        }
    }
    return nullptr;
}


void* GstDecode::gstBusFunc(void* arg)
{
    GstDecode* pStream = (GstDecode*)arg;
    pStream->runBusThread();
    pthread_exit(NULL);
}

void GstDecode::runBusThread()
{
    m_gstBus = gst_element_get_bus(m_gstPipeline);

    bool bRes = false;
    while (true)
    {
        GstMessage *gstMsg = gst_bus_timed_pop(m_gstBus, GST_CLOCK_TIME_NONE);
        bRes = gstBusProcessMsg(gstMsg);
        gst_message_unref(gstMsg);
        if (bRes == false)
        {
            break;
            m_bRun = false;
            sleep(5);
            gst_element_set_state(m_gstPipeline, GST_STATE_NULL);
            gst_object_unref(m_gstPipeline);
            gst_object_unref(m_gstAppSink);
            gst_object_unref(m_gstBus);
            makePipeline();
            m_gstBus = gst_element_get_bus(m_gstPipeline);
            m_bRun = true;
            continue;
        }
    }

    gst_object_unref(m_gstBus);
}

void* GstDecode::gstStreamFunc(void* arg)
{
    GstDecode* pStream = (GstDecode*)arg;
    pStream->runStreamThread();
    pthread_exit(NULL);
}

void GstDecode::runStreamThread()
{
}
