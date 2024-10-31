#ifndef AIModel_hpp
#define AIModel_hpp

#include "nlohmann/json.hpp"

#include "AbstractRuntime.hpp"
#include "RuntimeFactory.hpp"

class AIModel
{
    public:
        AIModel(nlohmann::json& jModelConfig, const std::string& strModelName);
        ~AIModel();

        template<typename T>
        void run(cv::Mat& mImage, T& va_arg)
        {
            m_pRuntime->run(mImage, va_arg);
        }
    
    private:
        AbstractRuntime* m_pRuntime = nullptr;
    
};

// template<typename T>
// void AIModel::run(cv::Mat& mImage, T& va_arg)
// {
//     m_pRuntime->run(mImage, va_arg);
// }

#endif // AIModel_hpp