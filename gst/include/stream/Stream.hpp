#ifndef Stream_hpp
#define Stream_hpp

#include <memory>
#include <string>
#include <map>
#include <mutex>

#include <opencv2/core/core.hpp>

#include "define/Types.hpp"

class Stream
{
    public:
        Stream() {}
        virtual ~Stream() = default;

        static std::shared_ptr<Stream> CreateStream(std::string strUrl);
        static std::shared_ptr<Stream> CreateStream(std::string strUrl, std::array<int, 4> arGrid);

        virtual std::string getUrl() = 0;
        virtual int getWidth() = 0;
        virtual int getHeight() = 0;
        virtual void getFrame(cv::Mat& mFrame) = 0;

        virtual void runStream() = 0;
        virtual void pauseStream() = 0;
        virtual void stopStream() = 0;
        virtual void join() = 0;

        virtual void setOverlay(std::vector<stObject_t>& stObjects) = 0;
        virtual void setOverlay(stObject_t& stObject) = 0;

};

#endif // Stream_hpp