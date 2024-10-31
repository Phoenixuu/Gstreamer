#include "ORTModel.hpp"

#include <iostream>

ORTModel::ORTModel()
{
    
}

ORTModel::ORTModel(nlohmann::json& jModelConfig, const std::string& strModelName) 
{
    m_strModelName = strModelName;
    std::string strModelPath = jModelConfig[JN_ASSETS_DIR].get<std::string>() + jModelConfig[m_strModelName][JN_ONNX_FILE].get<std::string>();
    
    Options_t stOptions;
    // stOptions.deviceIndex = jModelConfig[m_strModelName][JN_DEVICE_INDEX].get<int>();

    // std::string strPrecision = jModelConfig[m_strModelName][JN_PRECISION].get<std::string>();
    // if (strPrecision == "fp32")
    //     stOptions.precision = Precision_t::FP32;
    // else if (strPrecision == "fp16")
    //     stOptions.precision = Precision_t::FP16;
    // else if (strPrecision == "int8")
    //     stOptions.precision = Precision_t::INT8;
    // else
    //     stOptions.precision = Precision_t::FP16;

    // stOptions.optBatchSize = jModelConfig[m_strModelName][JN_OPT_PROFILE]["opt"].get<std::vector<int>>()[0];
    // stOptions.maxBatchSize = jModelConfig[m_strModelName][JN_OPT_PROFILE]["max"].get<std::vector<int>>()[0];
    stOptions.optWidth = jModelConfig[m_strModelName][JN_OPT_PROFILE]["opt"].get<std::vector<int>>()[2];
    stOptions.optHeight = jModelConfig[m_strModelName][JN_OPT_PROFILE]["opt"].get<std::vector<int>>()[1];
    // stOptions.minWidth = jModelConfig[m_strModelName][JN_OPT_PROFILE]["min"].get<std::vector<int>>()[2];
    // stOptions.minHeight = jModelConfig[m_strModelName][JN_OPT_PROFILE]["min"].get<std::vector<int>>()[1];
    // stOptions.maxWidth = jModelConfig[m_strModelName][JN_OPT_PROFILE]["max"].get<std::vector<int>>()[2];
    // stOptions.maxHeight = jModelConfig[m_strModelName][JN_OPT_PROFILE]["max"].get<std::vector<int>>()[1];

    shpORTRunner = std::make_shared<ORTRunner>(strModelPath, stOptions);

    shpORTRunner->getInputInfo(umpInputTensors, umpInputTensorsShape);
    shpORTRunner->getOutputInfo(umpOutputTensors);
}

ORTModel::~ORTModel()
{

}

void ORTModel::setScoreThreshold(float fScoreThreshold)
{
    m_fScoreThreshold = fScoreThreshold;
}

void ORTModel::setNMSThreshold(float fNMSThreshold)
{
    m_fNMSThreshold = fNMSThreshold;
}
