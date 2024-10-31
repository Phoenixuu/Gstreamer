#include "ORTRunner.hpp"

#include "common/Logger.hpp"

ORTRunner::ORTRunner(const std::string& strModelPath, Options_t& stOptions)
    : m_ortMemoryInfo(Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault))
{
    m_stOptions = stOptions;

    m_env = Ort::Env(OrtLoggingLevel::ORT_LOGGING_LEVEL_WARNING, strModelPath.c_str());
    m_sessionOptions = Ort::SessionOptions();
    m_session = new Ort::Session(m_env, strModelPath.c_str(), m_sessionOptions);

    size_t numInputNodes = m_session->GetInputCount();
    for (size_t i = 0; i < numInputNodes; i++)
    {
        Ort::AllocatedStringPtr inputNodeNameAllocated = m_session->GetInputNameAllocated(i, m_ortAllocator);
        const char* inputNodeName = std::move(inputNodeNameAllocated).release();
        m_inputNames.push_back(inputNodeName);

        m_inputTensorShape = {1, 3, m_stOptions.optHeight, m_stOptions.optWidth};
        size_t inputSize = vectorProduct(m_inputTensorShape);
        m_inputTensorSize = inputSize;

        m_umpInputTensors[inputNodeName] = {i, inputSize};
        m_umpInputTensorsShape[inputNodeName] = m_inputTensorShape;
    }

    size_t numOutputNodes = m_session->GetOutputCount();
    for (size_t i = 0; i < numOutputNodes; i++)
    {
        Ort::AllocatedStringPtr outputNodeNameAllocated = m_session->GetOutputNameAllocated(i, m_ortAllocator);
        const char* outputNodeName = std::move(outputNodeNameAllocated).release();
        m_outputNames.push_back(outputNodeName);

        m_umpOutputTensors[outputNodeName] = {i, 0};
    }

    // m_ortMemoryInfo = Ort::MemoryInfo::CreateCpu(OrtAllocatorType::OrtArenaAllocator, OrtMemType::OrtMemTypeDefault);
}

ORTRunner::~ORTRunner()
{
    delete m_session;
}

void ORTRunner::getInputInfo(std::unordered_map<std::string, std::vector<size_t>>& umpInputTensors, 
                            std::unordered_map<std::string, std::vector<int64_t>>& umpInputTensorsShape)
{
    umpInputTensors = m_umpInputTensors;
    umpInputTensorsShape = m_umpInputTensorsShape;
}

void ORTRunner::getOutputInfo(std::unordered_map<std::string, std::vector<size_t>>& umpOutputTensors)
{
    umpOutputTensors = m_umpOutputTensors;
}

void ORTRunner::runModel(std::vector<float>& inputOrtValues, std::vector<std::vector<float>>& outputOrtValues)
{
    std::vector<Ort::Value> ortInputTensors;
    ortInputTensors.push_back(
        Ort::Value::CreateTensor<float>(m_ortMemoryInfo, 
                                        inputOrtValues.data(), 
                                        m_inputTensorSize, 
                                        m_inputTensorShape.data(), 
                                        m_inputTensorShape.size()));
    
    std::vector<Ort::Value> ortOutputTensors;
    ortOutputTensors = m_session->Run(m_ortRunOptions, 
                                        m_inputNames.data(), 
                                        ortInputTensors.data(), 
                                        1, 
                                        m_outputNames.data(), 
                                        m_outputNames.size());

    outputOrtValues.clear();
    for (auto& tensor : ortOutputTensors)
    {
        auto* rawOutput = tensor.GetTensorData<float>();
        std::vector<int64_t> outputShape = tensor.GetTensorTypeAndShapeInfo().GetShape();
        size_t outputTensorSize = vectorProduct(outputShape);
        std::vector<float> outputTensor(rawOutput, rawOutput + outputTensorSize);
        outputOrtValues.push_back(outputTensor);
    }
}
