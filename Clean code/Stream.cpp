#include "Stream.hpp"

#include "<rtsp_decode.hpp>"

std::shared_ptr<Stream> Stream::CreateStream(std::string strUrl)
{
	std::shared_ptr<Stream> shpStream;
	
	shpStream = std::make_shared<rtsp_decode>(strUrl);
	
	return shpStream
}