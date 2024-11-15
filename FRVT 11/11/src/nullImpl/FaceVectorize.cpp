//
// Created by maicg on 3/25/23.
//
#include "FaceVectorize.h"
#include <cpu.h>
#include <iomanip>

int FaceVectorize::load(const std::string &param_file, const std::string &weight_file, bool use_gpu) {
    net_FaceVectorize.clear();
    int num_threads = 1;
    int powersave = 0;

    net_FaceVectorize.opt = ncnn::Option();
//         model_.opt.use_vulkan_compute = use_gpu;
#if NCNN_VULKAN
    net_FaceVectorize.opt.use_vulkan_compute = use_gpu;
#endif
    ncnn::set_cpu_powersave(powersave);

    ncnn::set_omp_dynamic(0);
    ncnn::set_omp_num_threads(num_threads);

    net_FaceVectorize.opt.lightmode = true;
    net_FaceVectorize.opt.num_threads = num_threads;

    if (net_FaceVectorize.load_param(param_file.c_str()) != 0) {
        std::cerr << "Failed to load model params from buffer." << std::endl;
        return -1;
    }
    if (net_FaceVectorize.load_model(weight_file.c_str()) != 0) {
        std::cerr << "Failed to load model params from buffer." << std::endl;
        return -2;
    }
    return 0;
}

std::vector<std::vector<float>> FaceVectorize::infer_FaceVectorize(cv::Mat& img) {
    int w = img.cols;
    int h = img.rows;
    std::vector<float> feature;
    std::vector<float> quality;
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(img.data, ncnn::Mat::PIXEL_BGR, w, h, 112, 112);
    
    // normalize -1 1
    float mean[3] = {127.5f, 127.5f, 127.5f};
    float norm[3] = {1/127.5f, 1/127.5f, 1/127.5f};
    in.substract_mean_normalize(mean, norm);
    // fprintf(stderr, "\n After Normalizing ");

    ncnn::Extractor ex = net_FaceVectorize.create_extractor();
    ex.set_num_threads(1);
    // ex.set_light_mode(true);
    // std::cout << "dt1" << std::endl;
	// ex.set_num_threads(2);
	
	ex.input("in0", in);
    // std::cout << "done3" << std::endl;
    // std::cout << in.w << std::endl;
    // std::cout << in.h << std::endl;
    // std::cout << in.c << std::endl;
    // std::cout << std::setprecision(8) << std::fixed << in.channel(2)[111*112 + 111] << std::endl;
    
	ncnn::Mat out;
    ncnn::Mat out2;
	ex.extract("out0", out);
    ex.extract("out1", out2);
	feature.resize(512);
	quality.resize(1);
    // std::cout << "vecto1" << std::endl;
	for (int j = 0; j < 512; j++)
	{
		feature[j] = out[j];
        // std::cout << feature[j] << std::endl;
	}
    for (int j = 0; j < 1; j++)
	{
		quality[j] = out2[j];
        // std::cout << "quality nhe: " << quality[j] << std::endl;
	}
    normalize(feature);
    std::vector<std::vector<float>> emb_quality_FaceVectorize = {feature, quality};
    return emb_quality_FaceVectorize;
}


void FaceVectorize::normalize(std::vector<float> &feature)
{
    float sum = 0;
    for (auto it = feature.begin(); it != feature.end(); it++)
        sum += (float)*it * (float)*it;
    sum = sqrt(sum);
    for (auto it = feature.begin(); it != feature.end(); it++)
        *it /= sum;
}

double cosine_similarity(std::vector<float> v1, std::vector<float> v2) {
    if (v1.size() != v2.size()) {
        throw std::invalid_argument("Vectors must be of same length.");
    }

    double dot_product = 0.0;
    double norm_v1 = 0.0;
    double norm_v2 = 0.0;

    for (size_t i = 0; i < v1.size(); ++i) {
        dot_product += v1[i] * v2[i];
        norm_v1 += std::pow(v1[i], 2);
        norm_v2 += std::pow(v2[i], 2);
    }

    double cosine_sim = dot_product / (std::sqrt(norm_v1) * std::sqrt(norm_v2));
    return cosine_sim;
}

