/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <algorithm>
#include <cstring>
#include <cstdlib>

#include <string>
#include <cmath>
#include <opencv2/opencv.hpp>

#include "nullimplfrvt11.h"

using namespace std;
using namespace FRVT;
using namespace FRVT_11;

double cosineSimilarity(float *A, float *B, unsigned int Vector_Length, unsigned int start)
{
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (unsigned int i = start; i < start + Vector_Length; ++i)
    {
        dot += A[i] * B[i];
        denom_a += A[i] * A[i];
        denom_b += B[i] * B[i];
    }
    // if (std::sqrt(denom_a) == 0 || std::sqrt(denom_b) == 0) {
    //     std::cout << "Denom error!!!!" << std::endl;
    // }
    double similarity = dot / (std::sqrt(denom_a) * std::sqrt(denom_b));
    if (similarity < 0)
    {
        similarity = 0;
    }

    return similarity;
}

NullImplFRVT11::NullImplFRVT11() {}

NullImplFRVT11::~NullImplFRVT11() {}

ReturnStatus
NullImplFRVT11::initialize(const std::string &configDir)
{
    std::string detectionBin = configDir + "/FaceDetection/FaceDetection_10.bin";
    std::string detectionParam = configDir + "/FaceDetection/FaceDetection_10.param";
    std::string vectorizeBin = configDir + "/FaceVectorize/FaceVectorize.bin";
    std::string vectorizeParam = configDir + "/FaceVectorize/FaceVectorize.param";
    // std::cout << vectorizeParam << std::endl;
    // std::cout << "done1" << std::endl;
    detection.load(detectionParam, detectionBin, false);
    vectorize.load(vectorizeParam, vectorizeBin, false);
    // std::cout << "Load model success!!" << std::endl;
    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRVT11::createFaceTemplate(
        const std::vector<FRVT::Image> &faces,
        TemplateRole role,
        std::vector<uint8_t> &templ,
        std::vector<EyePair> &eyeCoordinates)
{
    // std::cout << "test1" << std::endl;
    std::vector<float> fv; 
    std::vector<FaceObject> detectionInfo;
    for (unsigned int i = 0; i < faces.size(); i++){
        cv::Mat faceMat;
        std::vector<FaceObject> detectionInfo_;
        bool canDectect = true;

        if (faces[i].depth == 8)
        {

            // std::cout << "Depth: " << std::to_string(faces[i].depth) << std::endl;

            faceMat = cv::Mat(faces[i].height, faces[i].width, CV_8UC1, faces[i].data.get());
            cv::cvtColor(faceMat, faceMat, cv::COLOR_GRAY2BGR);
        }
        else if (faces[i].depth == 24)
        {

            // std::cout << "Depth: " << std::to_string(faces[i].depth) << std::endl;

            faceMat = cv::Mat(faces[i].height, faces[i].width, CV_8UC3, faces[i].data.get());
            cv::cvtColor(faceMat, faceMat, cv::COLOR_RGB2BGR);
        }

        // std::cout << "Face image size: " << faceMat.size() << std::endl;

        if (faces[i].depth == 8 || faces[i].depth == 24)
        {
            cv::Mat faceRGB;
            cv::cvtColor(faceMat, faceRGB, cv::COLOR_BGR2RGB);
            detection.detect(faceRGB, detectionInfo_);
            // std::cout << "Faces in image: " << detectionInfo_.size() << std::endl;
        }

        if (detectionInfo_.empty())
        {

            // std::cout << "Warning: can't detect image" << std::endl;

            canDectect = false;
            FaceObject faceObject;
            faceObject.rect.x = 0;
            faceObject.rect.y = 0;
            faceObject.rect.width = faces[i].width - 1;
            faceObject.rect.height = faces[i].height - 1;
            faceObject.prob = 0.0;
            for (size_t j = 0; j < 5; j++)
            {
                faceObject.landmark[j].x = j;
                faceObject.landmark[j].y = j;
            }

            detectionInfo_.push_back(faceObject);
        }

        cv::Mat cropImage;
        if (canDectect) {
            if (detectionInfo_.size() < 2){
                // std::cout << "log10: "<< std::endl;
                cropImage = alignFace.Process(faceMat, detectionInfo_[0]);
                detectionInfo.push_back(detectionInfo_[0]);
                // cv::imshow("Camera", cropImage);
                // cv::waitKey(0);
            } else {
                // std::cout << "log20: "<< std::endl;
                float maxArea = -1.0;
                int maxAreaIndex = -1;
                for (int j = 0; j < detectionInfo_.size(); j++) {
                    float computeAreaFace = detectionInfo_[j].rect.height*detectionInfo_[j].rect.width;
                    if (computeAreaFace > maxArea) {
                        maxArea = computeAreaFace; 
                        maxAreaIndex = j;
                    }
                }
                cropImage = alignFace.Process(faceMat, detectionInfo_[maxAreaIndex]);
                detectionInfo.push_back(detectionInfo_[maxAreaIndex]);
                // cv::imshow("Camera", cropImage);
                // cv::waitKey(0);
            }
            
        } else {
            // std::cout << "log21: "<< std::endl;
            cropImage = faceMat.clone();
            detectionInfo.push_back(detectionInfo_[0]);
        }
        // std::cout << "test2" << std::endl;

        std::vector<std::vector<float>> emb_quality = vectorize.infer_FaceVectorize(cropImage);
        std::vector<float> emb = emb_quality[0];
        std::vector<float> quality = emb_quality[1];
        // std::cout << "Quality: "<< std::endl;
        // std::cout << quality[0] << std::endl;
        // std::cout << "++++++"<< std::endl;
        
        fv.reserve(fv.size() + std::distance(emb.begin(), emb.end()));
        fv.insert(fv.end(), emb.begin(), emb.end());

        cropImage.release();
        faceMat.release();
        detectionInfo_.clear();
    }

    const uint8_t* bytes = reinterpret_cast<const uint8_t*>(fv.data());
    int dataSize = sizeof(float) * fv.size();
    templ.resize(dataSize);
    memcpy(templ.data(), bytes, dataSize);

    for (unsigned int i = 0; i < detectionInfo.size(); i++)
    {
        FaceObject obj = detectionInfo[i];
        eyeCoordinates.push_back(EyePair(true, true, static_cast<int>(obj.landmark[0].x),
                                         static_cast<int>(obj.landmark[0].y),
                                         static_cast<int>(obj.landmark[1].x),
                                         static_cast<int>(obj.landmark[1].y)));
    }

    // Clear memory
    fv.clear();
    detectionInfo.clear();

    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRVT11::createIrisTemplate(
        const std::vector<FRVT::Image> &irises,
        TemplateRole role,
        std::vector<uint8_t> &templ,
        std::vector<IrisAnnulus> &irisLocations)
{
    return ReturnStatus(ReturnCode::NotImplemented);
}

ReturnStatus
NullImplFRVT11::createFaceTemplate(
    const FRVT::Image &image,
    FRVT::TemplateRole role,
    std::vector<std::vector<uint8_t>> &templs,
    std::vector<FRVT::EyePair> &eyeCoordinates)
{
    cv::Mat faceMat;
    std::vector<FaceObject> detectionInfo_;
    bool canDectect = true;

    if (image.depth == 8)
    {

        // std::cout << "Depth: " << std::to_string(faces[i].depth) << std::endl;

        faceMat = cv::Mat(image.height, image.width, CV_8UC1, image.data.get());
        cv::cvtColor(faceMat, faceMat, cv::COLOR_GRAY2BGR);
    }
    else if (image.depth == 24)
    {

        // std::cout << "Depth: " << std::to_string(image.depth) << std::endl;

        faceMat = cv::Mat(image.height, image.width, CV_8UC3, image.data.get());
        cv::cvtColor(faceMat, faceMat, cv::COLOR_RGB2BGR);
    }

    // std::cout << "Face image size: " << faceMat.size() << std::endl;

    if (image.depth == 8 || image.depth == 24)
    {
        cv::Mat faceRGB;
        cv::cvtColor(faceMat, faceRGB, cv::COLOR_BGR2RGB);
        detection.detect(faceRGB, detectionInfo_);
        faceRGB.release();
    }

    if (detectionInfo_.empty())
    {

        // std::cout << "Warning: can't detect image single" << std::endl;

        canDectect = false;

        FaceObject faceObject;
        faceObject.rect.x = 0;
        faceObject.rect.y = 0;
        faceObject.rect.width = image.width - 1;
        faceObject.rect.height = image.height - 1;
        faceObject.prob = 0.0;
        for (size_t j = 0; j < 5; j++)
        {
            faceObject.landmark[j].x = j;
            faceObject.landmark[j].y = j;
        }

        detectionInfo_.push_back(faceObject);
    }

    if (canDectect) {
        cv::Mat img_draw = faceMat.clone();
        // detection.draw(img_draw, detectionInfo_);
        // cv::imshow("image whole image",img_draw);
        // cv::waitKey();
        int count_Face = 0;
        for (int j = 0; j < detectionInfo_.size(); j++) {
            std::vector<uint8_t> templ;
            cv::Mat cropImage = alignFace.Process(faceMat, detectionInfo_[j]);
            // cv::imshow("candetect",cropImage);
            // cv::waitKey();
            std::vector<std::vector<float>> emb_quality = vectorize.infer_FaceVectorize(cropImage);
            std::vector<float> emb = emb_quality[0];
            std::vector<float> quality = emb_quality[1];
            float computeAreaFace = detectionInfo_[j].rect.height*detectionInfo_[j].rect.width;
            if (count_Face == 0 || computeAreaFace > 32*32) {
                const uint8_t* bytes = reinterpret_cast<const uint8_t*>(emb.data());
                int dataSize = sizeof(float) * emb.size();
                templ.resize(dataSize);
                memcpy(templ.data(), bytes, dataSize);
                templs.push_back(templ);

                emb.clear();
                quality.clear();
                templ.clear();

                eyeCoordinates.push_back(EyePair(true, true, static_cast<int>(detectionInfo_[j].landmark[0].x),
                                            static_cast<int>(detectionInfo_[j].landmark[0].y),
                                            static_cast<int>(detectionInfo_[j].landmark[1].x),
                                            static_cast<int>(detectionInfo_[j].landmark[1].y)));
                count_Face = count_Face + 1;
            }
            cropImage.release();
        }

    } else {
        std::vector<uint8_t> templ;
        cv::Mat cropImage = faceMat.clone();
        std::vector<std::vector<float>> emb_quality = vectorize.infer_FaceVectorize(cropImage);
        std::vector<float> emb = emb_quality[0];
        std::vector<float> quality = emb_quality[1];

        cropImage.release();
        int j = 0;
        const uint8_t* bytes = reinterpret_cast<const uint8_t*>(emb.data());
        int dataSize = sizeof(float) * emb.size();
        templ.resize(dataSize);
        memcpy(templ.data(), bytes, dataSize);
        templs.push_back(templ);

        emb.clear();
        quality.clear();
        templ.clear();

        eyeCoordinates.push_back(EyePair(true, true, static_cast<int>(detectionInfo_[0].landmark[0].x),
                                        static_cast<int>(detectionInfo_[0].landmark[0].y),
                                        static_cast<int>(detectionInfo_[0].landmark[1].x),
                                        static_cast<int>(detectionInfo_[0].landmark[1].y)));
        
    }
    faceMat.release();
    detectionInfo_.clear();
    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRVT11::matchTemplates(
        const std::vector<uint8_t> &verifTemplate,
        const std::vector<uint8_t> &enrollTemplate,
        double &score)
{
    
    // std::cout << "Verify Template size: " << verifTemplate.size() << std::endl;
    // std::cout << "Enrollment Template size: " << enrollTemplate.size() << std::endl;

    float *enrollVector = (float *)enrollTemplate.data();
    float *verifVector = (float *)verifTemplate.data();

    score = cosineSimilarity(enrollVector, verifVector, this->featureVectorSize, 0);
    // std::cout << "***Score***" << std::endl;
    // std::cout << score << std::endl;

    return ReturnStatus(ReturnCode::Success);
}

std::shared_ptr<Interface>
Interface::getImplementation()
{
    return std::make_shared<NullImplFRVT11>();
}





