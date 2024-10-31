
#ifndef SNPERUNTIME_H
#define SNPERUNTIME_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEBuilder.hpp"
#include "SNPE/SNPEFactory.hpp"
#include "DlSystem/ITensor.hpp"
#include "DlSystem/ITensorFactory.hpp"
#include "DlSystem/RuntimeList.hpp"
#include "DlSystem/StringList.hpp"
#include "DlSystem/String.hpp"
#include "DlContainer/IDlContainer.hpp"

#define DSP_RUNTIME zdl::DlSystem::Runtime_t::DSP
#define GPU_RUNTIME zdl::DlSystem::Runtime_t::GPU_FLOAT16
#define CPU_RUNTIME zdl::DlSystem::Runtime_t::CPU

typedef struct snpeBuilders {
    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    zdl::DlSystem::Runtime_t runtime;
} snpeBuilders;

int SetAdspLibraryPath();
std::unique_ptr<zdl::SNPE::SNPE> setBuilderSNPE(std::string containerPath, std::vector<std::string> outputLayers,
                                                zdl::DlSystem::Runtime_t target_device);
zdl::DlSystem::Runtime_t checkRuntime(zdl::DlSystem::Runtime_t runtime);
std::unique_ptr<zdl::DlContainer::IDlContainer> loadContainerFromFile(std::string containerPath);
std::unique_ptr<zdl::SNPE::SNPE> setBuilderOptions(std::unique_ptr<zdl::DlContainer::IDlContainer>& container,
                                                   zdl::DlSystem::RuntimeList runtimeList,
                                                   zdl::DlSystem::StringList outputs);
std::unique_ptr<zdl::DlSystem::ITensor> convertMat2BgrFloat(std::unique_ptr<zdl::SNPE::SNPE>& snpe, const cv::Mat& img);

#endif
