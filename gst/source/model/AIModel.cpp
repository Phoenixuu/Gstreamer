#include "AIModel.hpp"

#include "common/Logger.hpp"

AIModel::AIModel(nlohmann::json& jModelConfig, const std::string& strModelName)
{
    std::string modelName = strModelName;
    std::string strFrameWork = jModelConfig[modelName][JN_FRAMEWORK].get<std::string>();
    m_pRuntime = RuntimeFactory::createRuntime(modelName, strFrameWork, jModelConfig);
    if (m_pRuntime == nullptr)
    {
        ERROR("createRuntime faild!");
        exit(0);
    }
}

AIModel::~AIModel()
{
    
}

// template<typename T>
// void AIModel::run(cv::Mat& mImage, T& va_arg)
// {
//     m_pRuntime->run(mImage, va_arg);
// }
