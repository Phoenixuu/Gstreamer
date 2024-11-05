// Now, the appsrc example, video2. Here we want to do the opposite to video1: read a frame from a video file with OpenCV’s VideoCapture and send it to the GStreamer pipeline to display on the screen with autovideosink. The pipeline is:

// appsrc name=mysrc format=time caps=video/x-raw,format=BGR ! videoconvert ! autovideosink sync=1

// The option format=time refers to timestamp format, NOT the image format from the caps! It is not required for video, but for some reason, it is required for audio appsrc, which will fail otherwise with rather obscure error messages (took me once a long time to figure this out).

// This pipeline looks nice, but unfortunately, it will not work. If we try to play it, GStreamer will complain about the frame size. Indeed, we did not specify the frame size (width+height) in the appsrc caps, and it does not have a default one, so there is no way it can negotiate a frame size with the downstream pipeline. But we don’t know the frame size until we open the input file with OpenCV! How to solve this predicament? One could in principle defer creating the pipeline until we know the frame size, but it turns out that it is enough to defer playing it. This is exactly what we do in the function codeThreadSrcV(). In this function, we first open the input file with OpenCV and get the frame size and FPS:
// Read more at https://www.it-jim.com/blog/gstreamer-cpp-tutorial/#info-block-2

VideoCapture video(data.fileName);

MY_ASSERT(video.isOpened());

int imW = (int) video.get(CAP_PROP_FRAME_WIDTH);

int imH = (int) video.get(CAP_PROP_FRAME_HEIGHT);

double fps = video.get(CAP_PROP_FPS);

MY_ASSERT(imW > 0 && imH > 0 && fps > 0);

// Next, we create proper caps for our appsrc and set them with the  g_object_set():

ostringstream oss;

oss << “video/x-raw,format=BGR,width=” << imW << “,height=” << imH <<
          “,framerate=” << int(lround(fps)) << “/1”;

cout << “CAPS=” << oss.str() << endl;

GstCaps *capsVideo = gst_caps_from_string(oss.str().c_str());

g_object_set(data.srcVideo, “caps”, capsVideo, nullptr);

gst_caps_unref(capsVideo);

// Now we can finally play the pipeline and start the infinite loop over frames:

MY_ASSERT(gst_element_set_state(data.pipeline, GST_STATE_PLAYING));
int frameCount = 0;

Mat frame;

for (;;) {
…
}

// Inside the loop, we wait for the next frame from VideoCapture:

video.read(frame);

if (frame.empty())

         break;

// We create a GStreamer buffer and copy the data there, again using the raw pointers frame.data and m.data:

int bufferSize = frame.cols * frame.rows * 3;

GstBuffer *buffer = gst_buffer_new_and_alloc(bufferSize);

GstMapInfo m;

gst_buffer_map(buffer, &m, GST_MAP_WRITE);

memcpy(m.data, frame.data, bufferSize);

gst_buffer_unmap(buffer, &m);

// Now we have to set up the timestamp. This is important because otherwise GStreamer would not be able to play this video at the 1x speed:

buffer->pts = uint64_t(frameCount  / fps * GST_SECOND);

// Finally, we “push” this buffer into our appsrc:

GstFlowReturn ret = gst_app_src_push_buffer(GST_APP_SRC(data.srcVideo), 
      buffer);

++frameCount;

// Once we have exited the loop (upon the end-of-file), we want to shut down the pipeline gracefully by sending it an end-of-stream message.

gst_app_src_end_of_stream(GST_APP_SRC(data.srcVideo));

// Now, the answer. The VideoCapture decodes the video file as fast as it can, which can be quite fast on modern computers. However, our GStreamer pipeline is slow due to the sync=1 options (1x playback). But the pipeline will not signal our C++ code to slow down, the frame loop will run fast pushing more and more frames into the appsrc built-in queue, taking a lot of RAM, and possibly even crashing the application if the video is long enough.

// This flaw (which is not obvious at all for beginners, by the way, did you guess it?) show how tricky designing pipelines (especially real-time ones) is, and how you should plan ahead and not code thoughtlessly. What is the solution? It’s obvious, we want the pipeline to signal when it wants data and when it doesn’t. Let’s register a couple of GLib-style signal callbacks on appsrc signals:

g_signal_connect(data.srcVideo, “need-data”, G_CALLBACK(startFeed), &data);

g_signal_connect(data.srcVideo, “enough-data”, G_CALLBACK(stopFeed), &data);

// Since GLib is C and not C++, we cannot use lambdas or std::function in callbacks, only good old functional pointers. We supply the pointer &data to our data structure to make it usable by the callback functions. The callback functions simply set a single data flag:

static void startFeed(GstElement *source, guint size, GoblinData *data) {

using namespace std;

if (!data->flagRunV) {

     cout << “startFeed !” << endl;

     data->flagRunV = true;

}

}

static void stopFeed(GstElement *source, GoblinData *data) {

using namespace std;

if (data->flagRunV) {

     cout << “stopFeed !” << endl;

     data->flagRunV = false;

}

}

// And now, we check this flag at the frame-processing loop and wait if the pipeline tells us to:

if (!data.flagRunV) {

         cout << “(wait)” << endl;

         this_thread::sleep_for(chrono::milliseconds(10));

         continue;

}
