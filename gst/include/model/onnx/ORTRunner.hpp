#ifndef ORTRunner_hpp
#define ORTRunner_hpp

#include <string>
#include <numeric>

#include <onnxruntime_cxx_api.h>

#include "AICommon.hpp"

template <typename T>
T vectorProduct(const std::vector<T>& v)
{
    return std::accumulate(v.begin(), v.end(), 1, std::multiplies<T>());
}

class ORTRunner
{
    public:
        ORTRunner(const std::string& strModelPath, Options_t& stOptions);
        ~ORTRunner();

        void getInputInfo(std::unordered_map<std::string, std::vector<size_t>>& umpInputTensors, 
                            std::unordered_map<std::string, std::vector<int64_t>>& umpInputTensorsShape);

        void getOutputInfo(std::unordered_map<std::string, std::vector<size_t>>& umpOutputTensors);

        void runModel(std::vector<float>& inputOrtValues, std::vector<std::vector<float>>& outputOrtValues);

    private:
        Options_t m_stOptions;

        Ort::Env m_env{nullptr};
        Ort::SessionOptions m_sessionOptions{nullptr};
        Ort::Session* m_session;

        Ort::AllocatorWithDefaultOptions m_ortAllocator;
        Ort::MemoryInfo m_ortMemoryInfo;
        Ort::RunOptions m_ortRunOptions{nullptr};

        std::vector<const char*> m_inputNames;
        std::vector<const char*> m_outputNames;

        std::vector<int64_t> m_inputTensorShape;
        size_t m_inputTensorSize;

        std::unordered_map<std::string, std::vector<size_t>> m_umpInputTensors;
        std::unordered_map<std::string, std::vector<int64_t>> m_umpInputTensorsShape;

        std::unordered_map<std::string, std::vector<size_t>> m_umpOutputTensors;
        // std::unordered_map<std::string, std::vector<int64_t>> m_umpOutputTensorsShape;
};

#endif // ORTRunner_hpp
