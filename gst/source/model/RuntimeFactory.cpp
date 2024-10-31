#include "RuntimeFactory.hpp"

#include "ORTYoloV8.hpp"

#include "common/Logger.hpp"

RuntimeFactory::RuntimeFactory()
{

}

RuntimeFactory::~RuntimeFactory()
{

}

AbstractRuntime* RuntimeFactory::createRuntime(std::string& strModel, std::string& strFrameWork, nlohmann::json &jConfigModel)
{
    if (strFrameWork == JN_FRAMEWORK_ONNX)
    {
        if (strModel == JN_MODEL_YOLOV8)
        {
            INFO("createRuntime: {} with {}", JN_MODEL_YOLOV8, JN_FRAMEWORK_ONNX);
            return new ORTYoloV8(jConfigModel);
        }
        else
        {
            ERROR("{} with {} not support!", strModel, JN_FRAMEWORK_ONNX);
            return nullptr;
        }
    }
    else
    {
        ERROR("framework: {} not support!", strFrameWork);
        return nullptr;
    }
}
