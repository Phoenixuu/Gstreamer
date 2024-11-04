// // PROCESSING LOOP 1:
// // Check for EOS:
// if(gst_app_sink_is_eos(GST_APP_SINK(data.sinkVideo))) break;

// // Wait for the sample (synchronous):
// GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(data.sinkVideo));

// // Get image witdth + heigHt from sample caps:
// GstCaps *caps = gst_sample_get_caps(sample);
// GstStructure *s = gst_caps_get_structure(caps,0);
// int imW, imH;
// MY_ASSERT(gst_structure_get_int(s,"width",&imW);
// MY_ASSERT(gst_structure_get_int(s,"height",&imH);

// // PROCESSING LOOP 2:
// // Buffer in GStreamer is a data packet, not queue
// // Get buffer with data, create a map to access it

// GstBuffer *buffer = gst_sample_get_buffer(sample);
// GstMapInfo m;
// MY_ASSERT(gst_buffer_map(buffer, &m, GST_MAP_READ));

// // Wrap the BGR data with cv::Mat:
// cv::MAT frame(imgH, imgW, CV_8UC3, (void *) m.data);

// // Free the memory:
// gst_buffer_unmap(buffer,&m);
// gst_sample_unref(sample);
