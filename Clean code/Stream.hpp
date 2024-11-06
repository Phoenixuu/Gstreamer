#ifndef Stream_hpp
#define Stream_hpp

#include<memory>
#include<string>
#include<map>
#include<mutex>

#include<opencv2/core/core.hpp>

class Stream {
public:
	Stream() {}
	virtual ~Stream() = default;

	static std::shared_ptr<Stream> CreateStream(std::string strUrl);

	virtual std::string getUrl() = 0;
	virtual int getWidth() = 0;
	virtual int getHeigh() = 0;
	virtual	void getFrame(cv::Mat& mFrame) = 0;

	virtual void runStream() = 0;
	virtual void pauseStream() = 0;
	virtual void stopStream() = 0;
	virtual void join() = 0;

};

#endif // Stream_hpp