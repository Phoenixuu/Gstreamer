//
// Created by hieptran on 3/25/23.
//

#include "FaceDetection.h"

#include <iostream>
#include <string.h>
#include <cmath>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "cpu.h"

static inline float intersection_area(const FaceObject& a, const FaceObject& b)
{
    cv::Rect_<float> inter = a.rect & b.rect;
    return inter.area();
}

static void qsort_descent_inplace(std::vector<FaceObject>& faceobjects, int left, int right)
{
    int i = left;
    int j = right;
    float p = faceobjects[(left + right) / 2].prob;

    while (i <= j)
    {
        while (faceobjects[i].prob > p)
            i++;

        while (faceobjects[j].prob < p)
            j--;

        if (i <= j)
        {
            // swap
            std::swap(faceobjects[i], faceobjects[j]);

            i++;
            j--;
        }
    }

//     #pragma omp parallel sections
    {
//         #pragma omp section
        {
            if (left < j) qsort_descent_inplace(faceobjects, left, j);
        }
//         #pragma omp section
        {
            if (i < right) qsort_descent_inplace(faceobjects, i, right);
        }
    }
}

static void qsort_descent_inplace(std::vector<FaceObject>& faceobjects)
{
    if (faceobjects.empty())
        return;

    qsort_descent_inplace(faceobjects, 0, faceobjects.size() - 1);
}

static void nms_sorted_bboxes(const std::vector<FaceObject>& faceobjects, std::vector<int>& picked, float nms_threshold)
{
    picked.clear();

    const int n = faceobjects.size();

    std::vector<float> areas(n);
    for (int i = 0; i < n; i++)
    {
        areas[i] = faceobjects[i].rect.area();
    }

    for (int i = 0; i < n; i++)
    {
        const FaceObject& a = faceobjects[i];

        int keep = 1;
        for (int j = 0; j < (int)picked.size(); j++)
        {
            const FaceObject& b = faceobjects[picked[j]];

            // intersection over union
            float inter_area = intersection_area(a, b);
            float union_area = areas[i] + areas[picked[j]] - inter_area;
            //             float IoU = inter_area / union_area
            if (inter_area / union_area > nms_threshold)
                keep = 0;
        }

        if (keep)
            picked.push_back(i);
    }
}

static ncnn::Mat generate_anchors(int base_size, const ncnn::Mat& ratios, const ncnn::Mat& scales)
{
    int num_ratio = ratios.w;
    int num_scale = scales.w;

    ncnn::Mat anchors;
    anchors.create(4, num_ratio * num_scale);

    const float cx = 0;
    const float cy = 0;

    for (int i = 0; i < num_ratio; i++)
    {
        float ar = ratios[i];

        int r_w = round(base_size / sqrt(ar));
        int r_h = round(r_w * ar); //round(base_size * sqrt(ar));

        for (int j = 0; j < num_scale; j++)
        {
            float scale = scales[j];

            float rs_w = r_w * scale;
            float rs_h = r_h * scale;

            float* anchor = anchors.row(i * num_scale + j);

            anchor[0] = cx - rs_w * 0.5f;
            anchor[1] = cy - rs_h * 0.5f;
            anchor[2] = cx + rs_w * 0.5f;
            anchor[3] = cy + rs_h * 0.5f;
        }
    }

    return anchors;
}

static void generate_proposals(const ncnn::Mat& anchors, int feat_stride, const ncnn::Mat& score_blob, const ncnn::Mat& bbox_blob, const ncnn::Mat& kps_blob, float prob_threshold, std::vector<FaceObject>& faceobjects)
{
    int w = score_blob.w;
    int h = score_blob.h;

    // generate face proposal from bbox deltas and shifted anchors
    const int num_anchors = anchors.h;

    for (int q = 0; q < num_anchors; q++)
    {
        const float* anchor = anchors.row(q);

        const ncnn::Mat score = score_blob.channel(q);
        const ncnn::Mat bbox = bbox_blob.channel_range(q * 4, 4);

        // shifted anchor
        float anchor_y = anchor[1];

        float anchor_w = anchor[2] - anchor[0];
        float anchor_h = anchor[3] - anchor[1];

        for (int i = 0; i < h; i++)
        {
            float anchor_x = anchor[0];

            for (int j = 0; j < w; j++)
            {
                int index = i * w + j;

                float prob = score[index];

                if (prob >= prob_threshold)
                {
                    // mmdet/models/dense_heads/FaceDetection_head.py _get_bboxes_single()
                    float dx = bbox.channel(0)[index] * feat_stride;
                    float dy = bbox.channel(1)[index] * feat_stride;
                    float dw = bbox.channel(2)[index] * feat_stride;
                    float dh = bbox.channel(3)[index] * feat_stride;

                    // mmdet/core/bbox/transforms.py distance2bbox()
                    float cx = anchor_x + anchor_w * 0.5f;
                    float cy = anchor_y + anchor_h * 0.5f;

                    float x0 = cx - dx;
                    float y0 = cy - dy;
                    float x1 = cx + dw;
                    float y1 = cy + dh;

                    FaceObject obj;
                    obj.rect.x = x0;
                    obj.rect.y = y0;
                    obj.rect.width = x1 - x0 + 1;
                    obj.rect.height = y1 - y0 + 1;
                    obj.prob = prob;

                    if (!kps_blob.empty())
                    {
                        const ncnn::Mat kps = kps_blob.channel_range(q * 10, 10);

                        obj.landmark[0].x = cx + kps.channel(0)[index] * feat_stride;
                        obj.landmark[0].y = cy + kps.channel(1)[index] * feat_stride;
                        obj.landmark[1].x = cx + kps.channel(2)[index] * feat_stride;
                        obj.landmark[1].y = cy + kps.channel(3)[index] * feat_stride;
                        obj.landmark[2].x = cx + kps.channel(4)[index] * feat_stride;
                        obj.landmark[2].y = cy + kps.channel(5)[index] * feat_stride;
                        obj.landmark[3].x = cx + kps.channel(6)[index] * feat_stride;
                        obj.landmark[3].y = cy + kps.channel(7)[index] * feat_stride;
                        obj.landmark[4].x = cx + kps.channel(8)[index] * feat_stride;
                        obj.landmark[4].y = cy + kps.channel(9)[index] * feat_stride;
                    }

                    faceobjects.push_back(obj);
                }

                anchor_x += feat_stride;
            }

            anchor_y += feat_stride;
        }
    }
}

// int FaceDetection::load(bool use_gpu, std::string &bin_file, std::string &param_file)
// {
//     net_FaceDetection.clear();

//     ncnn::set_cpu_powersave(2);
//     ncnn::set_omp_num_threads(ncnn::get_big_cpu_count());

//     net_FaceDetection.opt = ncnn::Option();

// #if NCNN_VULKAN
//     net_FaceDetection.opt.use_vulkan_compute = use_gpu;
// #endif

//     net_FaceDetection.opt.num_threads = ncnn::get_big_cpu_count();

//     // char parampath[256];
//     // char modelpath[256];
//     // sprintf(parampath, "FaceDetection_%s-opt2.param", modeltype);
//     // sprintf(modelpath, "FaceDetection_%s-opt2.bin", modeltype);

//     // FaceDetection.load_param(parampath);
//     // FaceDetection.load_model(modelpath);

//     net_FaceDetection.load_param(param_file.c_str());
//     net_FaceDetection.load_model(bin_file.c_str());


//     // has_kps = strstr(modeltype, "_kps") != NULL;
//     has_kps = true;
//     std::cout << has_kps << std::endl;

//     return 0;
// }

int FaceDetection::load(const std::string &param_file, const std::string &weight_file, bool use_gpu) {
    int num_threads = 1;
    int powersave = 0;
    net_FaceDetection.clear();

    net_FaceDetection.opt = ncnn::Option();
//         model_.opt.use_vulkan_compute = use_gpu;
#if NCNN_VULKAN
    net_FaceDetection.opt.use_vulkan_compute = use_gpu;
#endif
    ncnn::set_cpu_powersave(powersave);

    ncnn::set_omp_dynamic(0);
    ncnn::set_omp_num_threads(num_threads);

    net_FaceDetection.opt.lightmode = true;
    net_FaceDetection.opt.num_threads = num_threads;
    // const unsigned char* t = reinterpret_cast<const unsigned char *>( "123" );

    if (net_FaceDetection.load_param(param_file.c_str()) != 0) {
        std::cerr << "Failed to load model params from buffer." << std::endl;
        return -1;
    }
    if (net_FaceDetection.load_model(weight_file.c_str()) != 0) {
        std::cerr << "Failed to load model params from buffer." << std::endl;
        return -2;
    }
    has_kps = true;
    return 0;
}

int FaceDetection::detect(cv::Mat& rgb, std::vector<FaceObject>& faceobjects, float prob_threshold, float nms_threshold)
{
    // std::cout << "den0" << std::endl;

    int width = rgb.cols;
    int height = rgb.rows;

    const int target_size = 320;

    // pad to multiple of 32
    int w = width;
    int h = height;
    float scale = 1.f;
    if (w > h)
    {
        scale = (float)target_size / w;
        w = target_size;
        h = h * scale;
    }
    else
    {
        scale = (float)target_size / h;
        h = target_size;
        w = w * scale;
    }
    // std::cout << "den1" << std::endl;
    ncnn::Mat in = ncnn::Mat::from_pixels_resize(rgb.data, ncnn::Mat::PIXEL_RGB, width, height, w, h);

        // pad to target_size rectangle
        auto wpad = (w + 31) / 32 * 32 - w;
        auto hpad = (h + 31) / 32 * 32 - h;
        ncnn::Mat in_pad;
        ncnn::copy_make_border(in, in_pad, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, ncnn::BORDER_CONSTANT, 0.f);

        const float mean_vals[3] = {127.5f, 127.5f, 127.5f};
        const float norm_vals[3] = {1/128.f, 1/128.f, 1/128.f};
        in_pad.substract_mean_normalize(mean_vals, norm_vals);

        ncnn::Extractor ex = net_FaceDetection.create_extractor();

        ex.input("input.1", in_pad);

        std::vector<FaceObject> faceproposals;
        // std::cout << "den2" << std::endl;


        // stride 8
        {
            ncnn::Mat score_blob, bbox_blob, kps_blob;
            ex.extract("score_8", score_blob);
            ex.extract("bbox_8", bbox_blob);
            if (has_kps)
                ex.extract("kps_8", kps_blob);

            const int base_size = 16;
            const int feat_stride = 8;
            ncnn::Mat ratios(1);
            ratios[0] = 1.f;
            ncnn::Mat scales(2);
            scales[0] = 1.f;
            scales[1] = 2.f;
            ncnn::Mat anchors = generate_anchors(base_size, ratios, scales);

            std::vector<FaceObject> faceobjects32;
            generate_proposals(anchors, feat_stride, score_blob, bbox_blob, kps_blob, prob_threshold, faceobjects32);

            faceproposals.insert(faceproposals.end(), faceobjects32.begin(), faceobjects32.end());
        }
        // std::cout << "den3" << std::endl;

        // stride 16
        {
            ncnn::Mat score_blob, bbox_blob, kps_blob;
            ex.extract("score_16", score_blob);
            ex.extract("bbox_16", bbox_blob);
            if (has_kps)
                ex.extract("kps_16", kps_blob);

            const int base_size = 64;
            const int feat_stride = 16;
            ncnn::Mat ratios(1);
            ratios[0] = 1.f;
            ncnn::Mat scales(2);
            scales[0] = 1.f;
            scales[1] = 2.f;
            ncnn::Mat anchors = generate_anchors(base_size, ratios, scales);

            std::vector<FaceObject> faceobjects16;
            generate_proposals(anchors, feat_stride, score_blob, bbox_blob, kps_blob, prob_threshold, faceobjects16);

            faceproposals.insert(faceproposals.end(), faceobjects16.begin(), faceobjects16.end());
        }

        // stride 32
        {
            ncnn::Mat score_blob, bbox_blob, kps_blob;
            ex.extract("score_32", score_blob);
            ex.extract("bbox_32", bbox_blob);
            if (has_kps)
                ex.extract("kps_32", kps_blob);

            const int base_size = 256;
            const int feat_stride = 32;
            ncnn::Mat ratios(1);
            ratios[0] = 1.f;
            ncnn::Mat scales(2);
            scales[0] = 1.f;
            scales[1] = 2.f;
            ncnn::Mat anchors = generate_anchors(base_size, ratios, scales);

            std::vector<FaceObject> faceobjects8;
            generate_proposals(anchors, feat_stride, score_blob, bbox_blob, kps_blob, prob_threshold, faceobjects8);

            faceproposals.insert(faceproposals.end(), faceobjects8.begin(), faceobjects8.end());
        }

        // sort all proposals by score from highest to lowest
        qsort_descent_inplace(faceproposals);

        // apply nms with nms_threshold
        std::vector<int> picked;
        nms_sorted_bboxes(faceproposals, picked, nms_threshold);

        int face_count = picked.size();

        faceobjects.resize(face_count);
        for (int i = 0; i < face_count; i++)
        {
            faceobjects[i] = faceproposals[picked[i]];

            // adjust offset to original unpadded
            float x0 = (faceobjects[i].rect.x - (wpad / 2)) / scale;
            float y0 = (faceobjects[i].rect.y - (hpad / 2)) / scale;
            float x1 = (faceobjects[i].rect.x + faceobjects[i].rect.width - (wpad / 2)) / scale;
            float y1 = (faceobjects[i].rect.y + faceobjects[i].rect.height - (hpad / 2)) / scale;

            x0 = std::max(std::min(x0, (float)width - 1), 0.f);
            y0 = std::max(std::min(y0, (float)height - 1), 0.f);
            x1 = std::max(std::min(x1, (float)width - 1), 0.f);
            y1 = std::max(std::min(y1, (float)height - 1), 0.f);

            faceobjects[i].rect.x = x0;
            faceobjects[i].rect.y = y0;
            faceobjects[i].rect.width = x1 - x0;
            faceobjects[i].rect.height = y1 - y0;

            if (has_kps)
            {
                auto x0 = (faceobjects[i].landmark[0].x - (wpad / 2)) / scale;
                float y0 = (faceobjects[i].landmark[0].y - (hpad / 2)) / scale;
                float x1 = (faceobjects[i].landmark[1].x - (wpad / 2)) / scale;
                float y1 = (faceobjects[i].landmark[1].y - (hpad / 2)) / scale;
                float x2 = (faceobjects[i].landmark[2].x - (wpad / 2)) / scale;
                float y2 = (faceobjects[i].landmark[2].y - (hpad / 2)) / scale;
                float x3 = (faceobjects[i].landmark[3].x - (wpad / 2)) / scale;
                float y3 = (faceobjects[i].landmark[3].y - (hpad / 2)) / scale;
                float x4 = (faceobjects[i].landmark[4].x - (wpad / 2)) / scale;
                float y4 = (faceobjects[i].landmark[4].y - (hpad / 2)) / scale;

                faceobjects[i].landmark[0].x = std::max(std::min(x0, (float)width - 1), 0.f);
                faceobjects[i].landmark[0].y = std::max(std::min(y0, (float)height - 1), 0.f);
                faceobjects[i].landmark[1].x = std::max(std::min(x1, (float)width - 1), 0.f);
                faceobjects[i].landmark[1].y = std::max(std::min(y1, (float)height - 1), 0.f);
                faceobjects[i].landmark[2].x = std::max(std::min(x2, (float)width - 1), 0.f);
                faceobjects[i].landmark[2].y = std::max(std::min(y2, (float)height - 1), 0.f);
                faceobjects[i].landmark[3].x = std::max(std::min(x3, (float)width - 1), 0.f);
                faceobjects[i].landmark[3].y = std::max(std::min(y3, (float)height - 1), 0.f);
                faceobjects[i].landmark[4].x = std::max(std::min(x4, (float)width - 1), 0.f);
                faceobjects[i].landmark[4].y = std::max(std::min(y4, (float)height - 1), 0.f);
            }
        }

    return 0;
}

// int FaceDetection::draw(cv::Mat& rgb, const std::vector<FaceObject>& faceobjects)
// {
//     for (size_t i = 0; i < faceobjects.size(); i++)
//     {
//         FaceObject obj = faceobjects[i];

//         cv::rectangle(rgb, obj.rect, cv::Scalar(0, 255, 0));

//         if (has_kps)
//         {
//             cv::circle(rgb, obj.landmark[0], 2, cv::Scalar(255, 255, 0), -1);
//             cv::circle(rgb, obj.landmark[1], 2, cv::Scalar(255, 255, 0), -1);
//             cv::circle(rgb, obj.landmark[2], 2, cv::Scalar(255, 255, 0), -1);
//             cv::circle(rgb, obj.landmark[3], 2, cv::Scalar(255, 255, 0), -1);
//             cv::circle(rgb, obj.landmark[4], 2, cv::Scalar(255, 255, 0), -1);
//         }

//         char text[256];
//         sprintf(text, "%.1f%%", obj.prob * 100);

//         int baseLine = 0;
//         cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

//         int x = obj.rect.x;
//         int y = obj.rect.y - label_size.height - baseLine;
//         if (y < 0)
//             y = 0;
//         if (x + label_size.width > rgb.cols)
//             x = rgb.cols - label_size.width;

//         cv::rectangle(rgb, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)), cv::Scalar(255, 255, 255), -1);

//         cv::putText(rgb, text, cv::Point(x, y + label_size.height), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
//     }
//     // cv::imshow("left", rgb);
// 	// cv::waitKey(0);

//     return 0;
// }

int FaceDetection::draw(cv::Mat& rgb, FaceObject& obj, std::string &text)
{
    cv::rectangle(rgb, obj.rect, cv::Scalar(0, 255, 0));
    int baseLine = 0;
    cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

    int x = obj.rect.x;
    int y = obj.rect.y - label_size.height - baseLine;
    if (y < 0)
        y = 0;
    if (x + label_size.width > rgb.cols)
        x = rgb.cols - label_size.width;

    cv::rectangle(rgb, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)), cv::Scalar(255, 255, 255), -1);

    cv::putText(rgb, text, cv::Point(x, y + label_size.height), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
    return 0;
}

std::vector<bool> FaceDetection::process_kps(FaceObject& obj, float& area_base){
    float area_crop = obj.rect.height*obj.rect.width;
    cv::Point2f l_eye = obj.landmark[0];
    cv::Point2f r_eye = obj.landmark[1];
    cv::Point2f nose = obj.landmark[2];
    cv::Point2f l_mouth = obj.landmark[3];
    cv::Point2f r_mouth = obj.landmark[4];

    cv::Point2f center1 =  (l_eye + l_mouth)/2;
    cv::Point2f center2 = (r_eye + r_mouth)/2;
    cv::Point2f center_eye = (l_eye + r_eye)/2;
    cv::Point2f center_mouth = (l_mouth + r_mouth)/2;

    double distance12 = cv::norm(center2 - center1);
    double distance_nose1 = cv::norm(center1 - nose);
    double distance_nose2 = cv::norm(center2 - nose);
    double distance_center_eye_mouth = cv::norm(center_eye - center_mouth);
    double distance_nose_ceye = cv::norm(center_eye - nose);
    double distance_nose_cmouth = cv::norm(center_mouth - nose);
    double tl = 0;
    double tl1 = 0;
    
    if ((distance_nose1-distance_nose2) <= 0) {
        tl = distance_nose1/distance_nose2;
    }else{
        tl = distance_nose2/distance_nose1;
    }

    if ((distance_nose_ceye - distance_nose_cmouth) <= 0){
        tl1 = distance_nose_ceye/distance_nose_cmouth;
    }else{
        tl1 = distance_nose_cmouth/distance_nose_ceye;
    }
    
    std::cout << "dang chay process" << std::endl;
    bool should_run_recognition = false;
    bool should_run_recognition_in_collection = false;
    if (distance12 >= distance_nose1 && distance12 >= distance_nose2){
        if (distance_center_eye_mouth >= distance_nose_ceye && distance_center_eye_mouth >= distance_nose_cmouth){
            should_run_recognition = true; //anh nho nhung ti le khuon mat on thi auto nhan dien
            std::cout << "true should" << std::endl;
            if (tl >= 0.35 && tl1 >= 0.35) {
                should_run_recognition_in_collection = true; //muon so sanh trong tap data moi thi anh nho nhung can du dac trung
            } else if ((area_crop > 0) && ((area_base/area_crop) > (1080*1920)/(64*64))){
                should_run_recognition_in_collection = true;
            }
        }
    }

    std::vector<bool> result_adaface = {should_run_recognition, should_run_recognition_in_collection};
    return result_adaface;
}


cv::Mat alignment(cv::Mat& img, FaceObject& obj){
    int direction = 0;
    cv::Point2f point_3rd;
    cv::Mat crop_img = img(cv::Range(obj.rect.y,obj.rect.height+obj.rect.y), cv::Range(obj.rect.x,obj.rect.width+obj.rect.x));

    cv::Point2f l_eye = obj.landmark[0];
    cv::Point2f r_eye = obj.landmark[1];

    float left_eye_x = l_eye.x; 
    float left_eye_y = l_eye.y;
    float right_eye_x = r_eye.x; 
    float right_eye_y = r_eye.y;

    if (left_eye_y > right_eye_y){
        point_3rd.x = right_eye_x;
        point_3rd.y = left_eye_y;
        direction = -1;
        std::cout << "rotate to clock direction" << std::endl;
    }else{
        point_3rd.x = left_eye_x;
        point_3rd.y = right_eye_y;
        direction = 1;
    }

    double a = cv::norm(l_eye - point_3rd);
    double b = cv::norm(r_eye - l_eye);
    double c = cv::norm(r_eye - point_3rd);

    double cos_a = (b*b + c*c - a*a)/(2*b*c);
    double angle_radian = acos(cos_a);
    double angle = angle_radian * 180.0 / M_PI;

    if (direction == -1){
        angle = 90 - angle;
    }

    std::cout << "angle: " << angle << std::endl;
    cv::Mat rotate_img;
    cv::Point2f center(crop_img.cols/2.0f, crop_img.rows/2.0f);
    cv::Mat rot_mat = getRotationMatrix2D(center, direction*angle, 1.0);
    cv::warpAffine(crop_img, rotate_img, rot_mat, crop_img.size());

    cv::Mat rotate_resize;
    cv::resize(rotate_img, rotate_resize, cv::Size(112, 112), 0, 0, cv::INTER_CUBIC);

    // cv::imshow("crop_img", rotate_resize);
	// cv::waitKey(0);
    return rotate_resize;
}