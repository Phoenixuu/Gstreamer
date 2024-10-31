#include "ORTYoloV8.hpp"

ORTYoloV8::ORTYoloV8(nlohmann::json& jModelConfig)
    : ORTModel(jModelConfig, JN_MODEL_YOLOV8)
{
    m_iWidthModel = umpInputTensorsShape[m_strInputName][3];
    m_iHeightModel = umpInputTensorsShape[m_strInputName][2];
    
    m_iOutputHeight = 4 + 15 + m_iNumClasses;

    setScoreThreshold(jModelConfig[JN_MODEL_YOLOV8][JN_CONFIDENCE_THRESHOLD].get<float>());
    setNMSThreshold(jModelConfig[JN_MODEL_YOLOV8][JN_NMS_THRESHOLD].get<float>());
}

ORTYoloV8::~ORTYoloV8()
{
    
}

void ORTYoloV8::run(cv::Mat& mImage, std::vector<stObject_t>& stObjects)
{
    preprocess(mImage);
    shpORTRunner->runModel(inputOrtValues, outputOrtValues);
    postprocess(stObjects);
}

void ORTYoloV8::preprocess(cv::Mat& mImage)
{
    m_iInputWidth = mImage.cols;
    m_iInputHeight = mImage.rows;

    m_fRatioWidth = 1.0f / (m_iWidthModel / static_cast<float>(m_iInputWidth));
    m_fRatioHeight = 1.0f / (m_iHeightModel / static_cast<float>(m_iInputHeight));

    cv::Mat mInput;
    cv::resize(mImage, mInput, cv::Size(m_iWidthModel, m_iHeightModel), 0, 0, cv::INTER_LINEAR);

    cv::cvtColor(mInput, mInput, cv::COLOR_BGR2RGB);
    cv::Mat mFloat;
    mInput.convertTo(mFloat, CV_32FC3, 1.f / 255.f);

    cv::Mat mChannels[3];
    cv::split(mFloat, mChannels);

    inputOrtValues.clear();
    for (auto& channel : mChannels)
    {
        std::vector<float> fVec(channel.begin<float>(), channel.end<float>());
        inputOrtValues.insert(inputOrtValues.end(), fVec.begin(), fVec.end());
    }
}

void ORTYoloV8::postprocess(std::vector<stObject_t>& stObjects)
{
    stObjects.clear();

    std::vector<float> fOutput = outputOrtValues[umpOutputTensors[m_strOutputName][0]];

    std::vector<cv::Rect> bboxes;
    std::vector<float> scores;
    std::vector<int> classes;

    m_iOutputWidth = fOutput.size() / m_iOutputHeight;
    for (int i = 0; i < m_iOutputWidth; i++)
    {
        std::vector<float> fScores;
        for (int j = 4; j < m_iOutputHeight - 15; j++)
        {
            fScores.push_back(fOutput[m_iOutputWidth * j + i]);
        }
        auto maxScorePtr = std::max_element(fScores.begin(), fScores.end());
        float fScore = *maxScorePtr;
        int iId = std::distance(fScores.begin(), maxScorePtr);
        if (fScore >= m_fScoreThreshold)
        {
            float x = fOutput[m_iOutputWidth * 0 + i];
            float y = fOutput[m_iOutputWidth * 1 + i];
            float w = fOutput[m_iOutputWidth * 2 + i];
            float h = fOutput[m_iOutputWidth * 3 + i];

            float x0 = std::clamp((x - 0.5f * w) * m_fRatioWidth, 0.f, (float)m_iInputWidth);
            float y0 = std::clamp((y - 0.5f * h) * m_fRatioHeight, 0.f, (float)m_iInputHeight);
            float x1 = std::clamp((x + 0.5f * w) * m_fRatioWidth, 0.f, (float)m_iInputWidth);
            float y1 = std::clamp((y + 0.5f * h) * m_fRatioHeight, 0.f, (float)m_iInputHeight);

            cv::Rect_<float> bbox;
            bbox.x = x0;
            bbox.y = y0;
            bbox.width = x1 - x0;
            bbox.height = y1 - y0;

            bboxes.push_back(bbox);
            scores.push_back(fScore);
            classes.push_back(iId);
        }
    }

    std::vector<int> indices;
    cv::dnn::NMSBoxesBatched(bboxes, scores, classes, m_fScoreThreshold, m_fNMSThreshold, indices);

    int cnt = 0;
    for (auto& chosenIdx : indices)
    {
        stObject_t obj;
        obj.rfBox = bboxes[chosenIdx];
        obj.fScore = scores[chosenIdx];
        obj.iId = classes[chosenIdx];
        obj.strLabel = std::to_string(classes[chosenIdx]);
        stObjects.push_back(obj);

        cnt += 1;
    }
}
