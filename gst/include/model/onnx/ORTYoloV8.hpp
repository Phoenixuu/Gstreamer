#ifndef ORTYoloV8_hpp
#define ORTYoloV8_hpp

// #include <functional>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/dnn/dnn.hpp>

#include "ORTModel.hpp"
#include "AbstractRuntime.hpp"
#include "define/Types.hpp"

class ORTYoloV8 : public ORTModel, public AbstractRuntime
{
    public:
        ORTYoloV8(nlohmann::json& jModelConfig);
        ~ORTYoloV8();

        void run(cv::Mat& mImage, std::vector<stObject_t>& stObjects) override;

    protected:
        void preprocess(cv::Mat& mImage) override;
        // void postprocess() override;

    private:
        void postprocess(std::vector<stObject_t>& stObjects);

    private:
        std::string m_strInputName = "images";
        std::string m_strOutputName = "output0";

        int m_iNumClasses = 9;
        int m_iWidthModel, m_iHeightModel;
        int m_iOutputWidth, m_iOutputHeight;

        int m_iInputWidth, m_iInputHeight;
        float m_fRatioWidth, m_fRatioHeight;

        // float m_fScoreThreshold = 0.5f;
        // float m_fNMSThreshold = 0.4f;

        // std::function<void (const std::vector<stObject_t>&)> m_fnCallback;
};

#endif // ORTYoloV8_hpp