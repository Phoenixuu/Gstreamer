#include "Stream.hpp"

#include "common/Logger.hpp"
#include "GstDecode.hpp"
#include "GstOverlay.hpp"

std::shared_ptr<Stream> Stream::CreateStream(std::string strUrl)
{
    std::shared_ptr<Stream> shpStream;
    
    shpStream = std::make_shared<GstDecode>(strUrl);

    return shpStream;
}

std::shared_ptr<Stream> Stream::CreateStream(std::string strUrl, std::array<int, 4> arGrid)
{
    std::shared_ptr<Stream> shpStream;
    
    shpStream = std::make_shared<GstOverlay>(strUrl, arGrid);

    return shpStream;
}
