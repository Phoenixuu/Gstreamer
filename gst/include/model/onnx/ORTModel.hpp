#ifndef ORTModel_hpp
#define ORTModel_hpp

#include <opencv2/core/core.hpp>

#include "nlohmann/json.hpp"

#include "ORTRunner.hpp"
#include "define/JsonTypes.hpp"

class ORTModel
{
    public:
        ORTModel();
        ORTModel(nlohmann::json& jModelConfig, const std::string& strModelName);
        ~ORTModel();

    protected:
        virtual void preprocess(cv::Mat& mImage) = 0;
        // virtual void postprocess() = 0;
        virtual void setNMSThreshold(float fNMSThreshold);
        virtual void setScoreThreshold(float fScoreThreshold);

    protected:
        std::string m_strModelName;
        std::shared_ptr<ORTRunner> shpORTRunner;

        std::unordered_map<std::string, std::vector<size_t>> umpInputTensors;
        std::unordered_map<std::string, std::vector<int64_t>> umpInputTensorsShape;

        std::unordered_map<std::string, std::vector<size_t>> umpOutputTensors;

        std::vector<float> inputOrtValues;
        std::vector<std::vector<float>> outputOrtValues;

        float m_fScoreThreshold = 0.5f;
        float m_fNMSThreshold = 0.4f;

};

#endif // ORTModel_hpp
