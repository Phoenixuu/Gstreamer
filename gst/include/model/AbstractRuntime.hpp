#ifndef AbstractRuntime_hpp
#define AbstractRuntime_hpp

#include <opencv2/core/core.hpp>

#include "define/Types.hpp"

class AbstractRuntime
{
    public:
        virtual ~AbstractRuntime() {};

        virtual void run(cv::Mat& mImage, std::vector<stObject_t>& stObjects) {};
        
};

#endif // AbstractRuntime_hpp