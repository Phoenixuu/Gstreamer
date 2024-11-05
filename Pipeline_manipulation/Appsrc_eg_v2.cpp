VideoCapture video(data.fileName);

MY_ASSERT(video.isOpened());

int imW = (int) video.get(CAP_PROP_FRAME_WIDTH);

int imH = (int) video.get(CAP_PROP_FRAME_HEIGHT);

double fps = video.get(CAP_PROP_FPS);

MY_ASSERT(imW > 0 && imH > 0 && fps > 0);

Next, we create proper caps for our appsrc and set them with the  g_object_set():

ostringstream oss;

oss << “video/x-raw,format=BGR,width=” << imW << “,height=” << imH <<
          “,framerate=” << int(lround(fps)) << “/1”;

cout << “CAPS=” << oss.str() << endl;

GstCaps *capsVideo = gst_caps_from_string(oss.str().c_str());

g_object_set(data.srcVideo, “caps”, capsVideo, nullptr);

gst_caps_unref(capsVideo);

Now we can finally play the pipeline and start the infinite loop over frames:

MY_ASSERT(gst_element_set_state(data.pipeline, GST_STATE_PLAYING));
int frameCount = 0;

Mat frame;

for (;;) {
…
}

Inside the loop, we wait for the next frame from VideoCapture:

video.read(frame);

if (frame.empty())

         break;

We create a GStreamer buffer and copy the data there, again using the raw pointers frame.data and m.data:

int bufferSize = frame.cols * frame.rows * 3;

GstBuffer *buffer = gst_buffer_new_and_alloc(bufferSize);

GstMapInfo m;

gst_buffer_map(buffer, &m, GST_MAP_WRITE);

memcpy(m.data, frame.data, bufferSize);

gst_buffer_unmap(buffer, &m);

Now we have to set up the timestamp. This is important because otherwise GStreamer would not be able to play this video at the 1x speed:

buffer->pts = uint64_t(frameCount  / fps * GST_SECOND);

Finally, we “push” this buffer into our appsrc:

GstFlowReturn ret = gst_app_src_push_buffer(GST_APP_SRC(data.srcVideo), 
      buffer);

++frameCount;

Once we have exited the loop (upon the end-of-file), we want to shut down the pipeline gracefully by sending it an end-of-stream message.

gst_app_src_end_of_stream(GST_APP_SRC(data.srcVideo));
