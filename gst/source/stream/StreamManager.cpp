#include "StreamManager.hpp"

StreamManager& StreamManager::GetInstance()
{
    static StreamManager pInstance;
    return pInstance;
}

std::shared_ptr<Stream> StreamManager::getStream(std::string strUrl)
{
        const std::lock_guard<std::mutex> mtLock(m_mtLock);
        if (m_mpStreams.find(strUrl) == m_mpStreams.end())
        {
            m_iNumGrid += 1;
            if (m_iNumGrid == 4)
                m_iNumGrid = 1;
            
            #ifdef WAYLAND
                auto shpStream = Stream::CreateStream(strUrl, umpGrid4[m_iNumGrid]);
            #else
                auto shpStream = Stream::CreateStream(strUrl);
            #endif

            m_mpStreams[strUrl] = shpStream;
        }

        return m_mpStreams[strUrl];
}

void StreamManager::clean()
{

}
