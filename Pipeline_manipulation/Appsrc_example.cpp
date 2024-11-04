//pipeline
appsrc name = mysrc format=time caps=video/x-raw, format=BGR ! videoconvert! autovideosink sync=1

format=time //Not need for video, must have for audio!

//video 2
ostringstream oss;
oss << "video/x-raw,format=BGR,width=" << imW << ",height=" << imH << ",framerate=" << int(Iround(fps)) <<"/1";
GstCaps *capsVideo = gst_caps_from_string(oss.str().c_str());
g_object_set(data.srcVideo, "caps", capsVideo, nullptr);
gst_caps_unref(capsVideo);

//play pipeline
MY_ASSERT(gst_element_set_state(data.pipeline, GST_STATE_PLAYING));

//when get cv::Mat frame, let's convert to a GstBuffer:
int buffersize = frame.cols * frame.rows *3;
GstBuffer *buffer = gst_buffer_new_and_alloc(buffersize);
GstMapInfo m;
gst_buffer_map(buffer, &m, buffersize);
memcpy(m.data, frame.data, bufferSize);
gst_buffer_unmap(buffer, &m);

//add timestamp
buffer -> pts = uint64_t(frameCount / fps*GST_SECOND);

// finally, send to pipeline
gst_app_src_push_buffer(GST_APP_SRC(data.srcVideo), buffer);

//video 2 details 3:
/* Problem: sending frame too fast, filling up the queue!
Why don't we let the pipeline itself tell us when it wants data and when it doesn't?
We register two callbacks handling signals GLib-style*/
g_signal_connect(data.srcVideo, "need-data", G_CALLBACK(startFeed), &data);
g_signal_connect(data.srcVideo, "enough-data", G_CALLBACK(stopFeed), &data);

static void startFeed(GstElement *source, guint size, GoblinData *data){
	data -> flagRunV = true;
}
static void stopFeed(GstElement *source, GoblinData *data) {
	data -> flagRunV = false;
}

//Our thread with cv::VideoCapture() is not processing frames while flagRunV is false!