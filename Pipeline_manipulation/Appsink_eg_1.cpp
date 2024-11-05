struct GoblinData {

GstElement *pipeline = nullptr;

GstElement *sinkVideo = nullptr;

};

We create an instance of this structure in main(), create the pipeline, and find the appsink by its name (“mysink”):

GoblinData data;
string pipeStr = “filesrc location=” + fileName + ” ! decodebin ! videoconvert ! appsink
    name=mysink max-buffers=2 sync=1 caps=video/x-raw,format=BGR”;

GError *err = nullptr;

data.pipeline = gst_parse_launch(pipeStr.c_str(), &err);

checkErr(err);

MY_ASSERT(data.pipeline);

data.sinkVideo = gst_bin_get_by_name(GST_BIN (data.pipeline), “mysink”);

MY_ASSERT(data.sinkVideo);

Next, we play the pipeline:

MY_ASSERT(gst_element_set_state(data.pipeline, GST_STATE_PLAYING));

Now, we have to wait for the bus, which we now put into a separate thread, see the code for details:

thread threadBus([&data]() -> void {

     codeThreadBus(data.pipeline, data, “GOBLIN”);

});

You can extract data from appsink by using either signals or direct C API, we chose the latter. We process data in a separate thread which we now start.
thread threadProcess([&data]() -> void {

     codeThreadProcessV(data);

});

Finally, we wait for the threads to finish and stop the pipeline:

threadBus.join();

threadProcess.join();

gst_element_set_state(data.pipeline, GST_STATE_NULL);

gst_object_unref(data.pipeline);

Everything interesting happens in the function codeThreadProcessV(). It has an endless loop for (;;) { … } , which we will eventually break out of. What’s in the loop?

First, we check for EOS:

if (gst_app_sink_is_eos(GST_APP_SINK(data.sinkVideo))) {

         cout << “EOS !” << endl;

         break;

}

Next we pull the sample (a kind of data packet) synchronously, waiting if needed. For raw video, a sample is one video frame:

GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(data.sinkVideo));

if (sample == nullptr) {

         cout << “NO sample !” << endl;

         break;

}

Now, we want to know the frame size. It turns out, that the sample actually has caps (don’t confuse it with the pad caps), and we can find the frame size in there:

GstCaps *caps = gst_sample_get_caps(sample);

MY_ASSERT(caps != nullptr);

GstStructure *s = gst_caps_get_structure(caps, 0);

int imW, imH;

MY_ASSERT(gst_structure_get_int(s, “width”, &imW));

MY_ASSERT(gst_structure_get_int(s, “height”, &imH));

cout << “Sample: W = ” << imW << “, H = ” << imH << endl;

Next, we extract a buffer (a lower-level data packet) from the sample. Note: in GStreamer slang, a “buffer” always means a “data packet”, and never ever a “queue”!

GstBuffer *buffer = gst_sample_get_buffer(sample);

Still, we don’t have a pointer to the raw data. For that we need a map:

GstMapInfo m;

MY_ASSERT(gst_buffer_map(buffer, &m, GST_MAP_READ));

MY_ASSERT(m.size == imW * imH * 3);

Now we can finally read the raw data (BRG pixels) via the pointer m.data. But we want to process the frame in OpenCV, so we wrap it in a cv::Mat.

cv::Mat frame(imH, imW, CV_8UC3, (void *) m.data);

Warning! Such a cv::Mat object does not copy the data, so if you want cv::Mat to persist when the GStreamer data packet is no more, or if you want to modify it, then clone it. Here we don’t have to (but we DO clone in video3). Now we can do anything we want with the cv::Mat image, but in this example, we just display it on the screen:

cv::imshow(“frame”, frame);

int key = cv::waitKey(1);

Now, we release the sample, and check if the ESC key was pressed:

gst_buffer_unmap(buffer, &m);

gst_sample_unref(sample);

if (27 == key)

         exit(0);
Read more at https://www.it-jim.com/blog/gstreamer-cpp-tutorial/#info-block-1