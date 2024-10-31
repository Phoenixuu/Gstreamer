#include "gstObject.hpp"
#include "json.hpp"
#include "cairo/cairo.h"
#include <filesystem>

using json = nlohmann::json;
#define RECOGNITION_THRESHOLD 0.95

static gboolean draw_callback(GstElement *overlay, cairo_t *cr, gint64 timestamp, gint64 duration, gpointer user_data) {
    // Your custom drawing logic here using the Cairo library
    cairo_set_source_rgba(cr, 1.0, 0.0, 0.0, 1.0);  // Set color to red
    cairo_rectangle(cr, 50, 50, 100, 100);  // Example: Draw a red rectangle
    cairo_fill(cr);
    return TRUE;
}

int gstObject::loadDB(std::string jsonFilePath) {
    std::ifstream jsonFile(jsonFilePath);
    if (!jsonFile.is_open()) {
        std::cout << "Database was not exist." << std::endl;
        return 1;
    }
    std::vector<std::vector<float>> emb;
    std::string line;
    while (std::getline(jsonFile, line)) {
        json jsonData = json::parse(line);
        std::string id = jsonData["id"];
        ids.push_back(id);
        std::vector<float> embVector = jsonData["emb"];
        emb.push_back(embVector);
    }
    for (uint32_t i = 0; i < emb.size(); i++) {
        feat.push_back(emb[i]);
    }
    feat = feat.reshape(1, emb.size());
    jsonFile.close();
    std::cout << "Database has " << ids.size() << " faces" << std::endl;
    return 0;
}

cv::Mat resizeKeepAspectRatio(const cv::Mat &input, const cv::Size &dstSize, const cv::Scalar &bgcolor) {
    cv::Mat output;
    double h1 = dstSize.width * (input.rows / (double)input.cols);
    double w2 = dstSize.height * (input.cols / (double)input.rows);
    if (h1 <= dstSize.height) {
        cv::resize(input, output, cv::Size(dstSize.width, h1));
    } else {
        cv::resize(input, output, cv::Size(w2, dstSize.height));
    }
    int top = (dstSize.height - output.rows) / 2;
    int down = (dstSize.height - output.rows + 1) / 2;
    int left = (dstSize.width - output.cols) / 2;
    int right = (dstSize.width - output.cols + 1) / 2;
    cv::copyMakeBorder(output, output, top, down, left, right, cv::BORDER_CONSTANT, bgcolor);
    return output;
}

int gstObject::addDB(std::string imgFilePath) {
    SetAdspLibraryPath();
    zdl::DlSystem::Runtime_t runtime = DSP_RUNTIME;
    runtime = checkRuntime(runtime);
    std::string folderPath = DB_IMAGE_PATH;
    std::ofstream jsonFile;
    jsonFile.open(DB_PATH, std::ios::app);
    // SCRFD* det = new SCRFD();
    // SnpeInsightface* rec = new SnpeInsightface();
    // det->load(FACEDET_MODEL_PATH, runtime);
    // rec->load(FACEREC_MODEL_PATH, runtime);
    for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
        std::vector<FaceObject> faces;
        std::string filePath = entry.path().string();
        std::string fileName = entry.path().filename().string();
        cv::Mat img = cv::imread(filePath);
        img = resizeKeepAspectRatio(img, cv::Size(640, 640), cv::Scalar(0, 0, 0));
        // copyMakeBorder(img, img, img.rows / 10, img.rows / 10, img.cols / 4, img.cols / 4, cv::BORDER_CONSTANT,
        //                cv::Scalar(0));
        det->execDetect(img, faces, 0.3, 0.45);
        if (faces.size() != 1) {
            std::cout << "-- " << fileName << " currently has " << faces.size() << " face. Must has exactly 1 face"
                      << std::endl;
            std::filesystem::remove(filePath);
            continue;
        } else {
            FaceObject face = faces[0];
            float v2[5][2] = {{float(face.point[0].x), float(face.point[0].y)},
                              {float(face.point[1].x), float(face.point[1].y)},
                              {float(face.point[2].x), float(face.point[2].y)},
                              {float(face.point[3].x), float(face.point[3].y)},
                              {float(face.point[4].x), float(face.point[4].y)}};
            cv::Mat src(5, 2, CV_32FC1, norm_face);
            cv::Mat dst(5, 2, CV_32FC1, v2);
            cv::Mat m = similarTransform(dst, src);
            cv::Size size(112, 112);
            cv::Mat aligned(112, 112, CV_32FC3);
            cv::Mat transfer = m(cv::Rect(0, 0, 3, 2));
            cv::warpAffine(img, aligned, transfer, size, 1, 0, 0);
            cv::Mat output = rec->execRecog(aligned);
            json resultJson;
            std::vector<float> vec;
            vec.assign(output.begin<float>(), output.end<float>());
            resultJson["emb"] = vec;
            resultJson["id"] = fileName.substr(0, fileName.find_last_of('.'));
            jsonFile << resultJson.dump() << std::endl;
            std::cout << "-- " << fileName << " added to db successfully" << std::endl;
        }
    }
    jsonFile.close();
    return 0;
}

gstObject::gstObject(std::string url, int inputType, int i) {
    threadID = i;
    SetAdspLibraryPath();
    zdl::DlSystem::Runtime_t runtime = DSP_RUNTIME;
    runtime = checkRuntime(runtime);
    det->load(FACEDET_MODEL_PATH, runtime);
    rec->load(FACEREC_MODEL_PATH, runtime);
    threadID = i;
    gst_init(nullptr, nullptr);
    if (inputType == INPUT_TYPE::VIDEO) {
        // std::string tmp = "uridecodebin uri=file://" + url +
        //                   " ! videoconvert ! video/x-raw,format=I420 ! appsink name=sink sync=false";
        std::string tmp = "filesrc location=" + url +
                          " ! qtdemux ! queue ! h264parse ! qtivdec ! videoconvert ! video/x-raw,format=I420 ! appsink name=sink sync=false";
        pipeline_ = gst_parse_launch(tmp.c_str(), nullptr);
    } else if (inputType == INPUT_TYPE::RTSP) {
        // pipeline_ = gst_parse_launch("rtspsrc location=rtsp://192.169.1.53/stream1 ! rtph264depay ! h264parse ! qtivdec ! videoconvert ! video/x-raw,format=I420 ! appsink name=sink sync=false", NULL);
        // pipeline_ = gst_parse_launch("rtspsrc location=rtsp://192.169.1.53/stream1 latency=0 ! rtph264depay ! avdec_h264 ! queue ! video/x-raw,format=I420 ! appsink name=sink sync=false", NULL);
        std::string tmp = "rtspsrc location=" + url +
                          " ! rtph264depay ! h264parse ! qtivdec ! videoconvert ! video/x-raw,format=I420 ! appsink name=sink sync=false";
        pipeline_ = gst_parse_launch(tmp.c_str(), nullptr);
    } else {
    }
    appsink_ = GST_APP_SINK(gst_bin_get_by_name(GST_BIN(pipeline_), "sink"));
    g_object_set(G_OBJECT(appsink_), "emit-signals", TRUE, nullptr);
    g_signal_connect(appsink_, "new-sample", G_CALLBACK(onNewSampleStatic), this);
    bus_ = gst_pipeline_get_bus(GST_PIPELINE(pipeline_));
    gst_bus_add_signal_watch(bus_);
    g_signal_connect(bus_, "message::eos", G_CALLBACK(onEosStatic), this);
}

gstObject::~gstObject() {
    gst_element_set_state(pipeline_, GST_STATE_NULL);
    gst_object_unref(pipeline_);
    gst_object_unref(bus_);
}

GstFlowReturn gstObject::onNewSample(GstElement* appsink) {
    GstSample* sample = gst_app_sink_pull_sample(GST_APP_SINK(appsink));
    GstCaps* caps = gst_sample_get_caps(sample);
    int width = 0, height = 0;
    if (caps) {
        GstStructure* structure = gst_caps_get_structure(caps, 0);
        gst_structure_get_int(structure, "width", &width);
        gst_structure_get_int(structure, "height", &height);
        // g_print("Received frame with width: %d, height: %d.\n", width, height);
    }
    buffer = gst_sample_get_buffer(sample);
    if (gst_buffer_map(buffer, &map_info, GST_MAP_READ)) {
        const guint8* raw_data = map_info.data;
        const gchar* format_string = gst_structure_get_string(gst_caps_get_structure(caps, 0), "format");
        std::string pixel_format(format_string ? format_string : "");
        cv::Mat frame;
        if (pixel_format == "BGR") {
            frame = cv::Mat(height, width, CV_8UC3, const_cast<guint8*>(raw_data));
        } else if (pixel_format == "RGB") {
            frame = cv::Mat(height, width, CV_8UC3, const_cast<guint8*>(raw_data));
        } else if (pixel_format == "I420") {
            cv::Mat yuv(height + height / 2, width, CV_8UC1, const_cast<guint8*>(raw_data));
            cv::cvtColor(yuv, frame, cv::COLOR_YUV2BGR_I420);
        } else if (pixel_format == "NV12") {
            cv::Mat yuv(height + height / 2, width, CV_8UC1, const_cast<guint8*>(raw_data));
            cv::cvtColor(yuv, frame, cv::COLOR_YUV2BGR_NV12);
        } else if (pixel_format == "BGRx") {
            frame = cv::Mat(height, width, CV_8UC4, const_cast<guint8*>(raw_data));
            cv::cvtColor(frame, frame, cv::COLOR_BGRA2BGR);
        } else {
            g_printerr("Unsupported pixel format: %s\n", pixel_format.c_str());
            gst_buffer_unmap(buffer, &map_info);
            gst_sample_unref(sample);
            return GST_FLOW_OK;
        }
        cv::Mat img = frame.clone();
        std::vector<FaceObject> faces;
        std::cout << "=============================================" << std::endl;
        auto t1 = std::chrono::high_resolution_clock::now();
        det->execDetect(img, faces);
        auto t2 = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
        std::cout << "Thread " << threadID << ": FaceDet takes " << ms_int.count() << "ms\n";
        std::cout << "Thread " << threadID << ": Frame has " << faces.size() << " faces" << std::endl;
        for (uint32_t i = 0; i < faces.size(); i++) {
            faceObjs.clear();
            auto t1 = std::chrono::high_resolution_clock::now();
            FaceObject face = faces[i];
            float v2[5][2] = {{float(face.point[0].x), float(face.point[0].y)},
                              {float(face.point[1].x), float(face.point[1].y)},
                              {float(face.point[2].x), float(face.point[2].y)},
                              {float(face.point[3].x), float(face.point[3].y)},
                              {float(face.point[4].x), float(face.point[4].y)}};
            cv::Mat src(5, 2, CV_32FC1, norm_face);
            cv::Mat dst(5, 2, CV_32FC1, v2);
            cv::Mat m = similarTransform(dst, src);
            cv::Size size(112, 112);
            cv::Mat aligned(112, 112, CV_32FC3);
            cv::Mat transfer = m(cv::Rect(0, 0, 3, 2));
            cv::warpAffine(img, aligned, transfer, size, 1, 0, 0);
            cv::Mat output = rec->execRecog(aligned);
            if (!feat.empty()) {
                class_info result = rec->classify(output, feat);
                if (result.min_distance < RECOGNITION_THRESHOLD) {
                    face.label = ids[result.index];
                } else {
                    face.label = "Unknown";
                }
                std::cout << face.label << " " << result.min_distance << std::endl;
            }
            faceObjs.push_back(face);
            // cv::imshow("win1", img);
            auto t2 = std::chrono::high_resolution_clock::now();
            auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            std::cout << "Thread " << threadID << ": FaceRec takes " << ms_int.count() << "ms\n";
        }
        gst_buffer_unmap(buffer, &map_info);
    }
    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

void gstObject::decode() {
    mainLoop_ = g_main_loop_new(nullptr, FALSE);
    gst_element_set_state(pipeline_, GST_STATE_PLAYING);
    g_main_loop_run(mainLoop_);
}