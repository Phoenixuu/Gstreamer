#include <X11/Xlib.h>
#include <gst/gst.h>
#include <gst/app/gstappsink.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

class VideoDecoder {
public:
    VideoDecoder(const std::string& rtsp_uri) : count(0) {
        gst_init(nullptr, nullptr);
        pipeline = gst_pipeline_new("video-decoder");
        if (!pipeline) {
            std::cerr << "Failed to create pipeline\n";
            return;
        }

        // Create GStreamer elements
        GstElement *source = gst_element_factory_make("rtspsrc", "source");
        GstElement *decodebin = gst_element_factory_make("decodebin", "decodebin");
        GstElement *tee = gst_element_factory_make("tee", "tee");
        GstElement *queue1 = gst_element_factory_make("queue", "queue1");
        GstElement *queue2 = gst_element_factory_make("queue", "queue2");
        GstElement *sink = gst_element_factory_make("appsink", "sink");
        GstElement *videosink = gst_element_factory_make("autovideosink", "videosink");

        if (!source || !decodebin || !tee || !queue1 || !queue2 || !sink || !videosink) {
            std::cerr << "Failed to create elements\n";
            return;
        }

        // Set RTSP source location
        g_object_set(source, "location", rtsp_uri.c_str(), "latency", 0, nullptr);
        
        g_object_set(decodebin, "low-percent", 0, nullptr);

        // Configure appsink
        g_object_set(sink, "emit-signals", TRUE, "sync", FALSE, nullptr);
        g_signal_connect(sink, "new-sample", G_CALLBACK(on_new_sample), this);

        // Add elements to the pipeline
        gst_bin_add_many(GST_BIN(pipeline), source, decodebin, tee, queue1, sink, queue2, videosink, nullptr);

        // Manually link decodebin to tee on pad-added signal
        g_signal_connect(decodebin, "pad-added", G_CALLBACK(on_pad_added), tee);

        // Manually link tee to queues and their respective sinks
        GstPad *tee_src_pad1 = gst_element_request_pad_simple(tee, "src_%u");
        GstPad *tee_src_pad2 = gst_element_request_pad_simple(tee, "src_%u");
        GstPad *queue1_sink_pad = gst_element_get_static_pad(queue1, "sink");
        GstPad *queue2_sink_pad = gst_element_get_static_pad(queue2, "sink");

        if (gst_pad_link(tee_src_pad1, queue1_sink_pad) != GST_PAD_LINK_OK ||
            gst_pad_link(tee_src_pad2, queue2_sink_pad) != GST_PAD_LINK_OK) {
            std::cerr << "Failed to link tee pads to queues\n";
            return;
        }

        gst_object_unref(queue1_sink_pad);
        gst_object_unref(queue2_sink_pad);
        gst_object_unref(tee_src_pad1);
        gst_object_unref(tee_src_pad2);

        // Link queues to their respective sinks
        if (!gst_element_link(queue1, sink) || !gst_element_link(queue2, videosink)) {
            std::cerr << "Failed to link queue to sink\n";
            return;
        }

        // Handle pad-added signal for rtspsrc and link to decodebin
        g_signal_connect(source, "pad-added", G_CALLBACK(on_pad_added), decodebin);

        // Start the pipeline
        gst_element_set_state(pipeline, GST_STATE_PLAYING);

        std::cout << "============Khoi tao done==============" << std::endl;
    }

    ~VideoDecoder() {
        if (pipeline) {
            gst_element_set_state(pipeline, GST_STATE_NULL);
            gst_object_unref(GST_OBJECT(pipeline));
        }
    }

    static GstFlowReturn on_new_sample(GstElement *sink, VideoDecoder *decoder) {
        GstSample *sample;
        g_signal_emit_by_name(sink, "pull-sample", &sample);
        if (sample) {
            GstCaps *caps = gst_sample_get_caps(sample);
            GstStructure *structure = gst_caps_get_structure(caps, 0);

            int width, height;
            if (!gst_structure_get_int(structure, "width", &width) ||
                !gst_structure_get_int(structure, "height", &height)) {
                std::cerr << "Failed to get width or height from caps\n";
                gst_sample_unref(sample);
                return GST_FLOW_ERROR;
            }

            const gchar *format = gst_structure_get_string(structure, "format");
            if (format == nullptr) {
                std::cerr << "Failed to get format from caps\n";
                gst_sample_unref(sample);
                return GST_FLOW_ERROR;
            }

            GstBuffer *buffer = gst_sample_get_buffer(sample);
            GstMapInfo map;
            
            std::cout << "==============GstSample===================" << std::endl;

            if (gst_buffer_map(buffer, &map, GST_MAP_READ)) {
                if (g_strcmp0(format, "I420") == 0) {
                    // I420 format: YUV420 Planar
                    size_t y_size = width * height;
                    size_t uv_size = y_size / 4;
                    
                    if (map.size >= y_size + 2 * uv_size) {
                        cv::Mat yuv(height + height / 2, width, CV_8UC1, map.data);
                        cv::Mat bgr(height, width, CV_8UC3);
                        cv::cvtColor(yuv, bgr, cv::COLOR_YUV2BGR_I420);
                        // cv::imwrite("/home/dunggps/Gstreamer/Output/" + std::to_string(decoder->count) + ".jpg", bgr);
                        decoder->count++;
                        std::cout << decoder->count << std::endl;
                        // cv::imshow("Video Stream", bgr);
                        // cv::waitKey(30); 
                    } else {
                        std::cerr << "Buffer size mismatch for I420 format\n";
                    }
                } else {
                    std::cerr << "Unsupported format: " << format << std::endl;
                }

                gst_buffer_unmap(buffer, &map);
            }
            gst_sample_unref(sample);
            return GST_FLOW_OK;
        }
        return GST_FLOW_ERROR;

        std::cout << "Hoan thanh imshow va co du lieu" << std::endl;
    }

private:
    GstElement *pipeline = nullptr;
    int count;


    static void on_pad_added(GstElement *src, GstPad *new_pad, gpointer user_data) {
        GstElement *target_element = GST_ELEMENT(user_data);
        GstPad *sink_pad = gst_element_get_static_pad(target_element, "sink");
        if (gst_pad_is_linked(sink_pad)) {
            gst_object_unref(sink_pad);
            return;
        }

        GstPadLinkReturn ret = gst_pad_link(new_pad, sink_pad);
        if (GST_PAD_LINK_FAILED(ret)) {
            std::cerr << "Failed to link elements\n";
        }

        gst_object_unref(sink_pad);
    }
};

int main(int argc, char *argv[]) {
    // Initialize X11 threads
    std::cout << "=============Vao main==============" << std::endl;
    XInitThreads();
    std::cout << "=============chay XInitThreads==========" << std::endl;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <RTSP URI>\n";
        return -1;
    }

    std::string rtsp_uri = argv[1];
    VideoDecoder decoder(rtsp_uri);
    std::cout << "===========Hoan thanh decoder==============" << std::endl;
    GMainLoop *loop = g_main_loop_new(nullptr, FALSE);
    std::cout << "===============chay ham loop===============" <<std::endl;
    g_main_loop_run(loop);
    std::cout << "==============End vong loop==============" << std::endl;

    return 0;
}
