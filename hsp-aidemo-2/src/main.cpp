#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <chrono>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "gstObject.hpp"

#define BUILD_X86
#define TEST_VIDEO
#define NUM_THREADS 8

std::vector<std::string> rtspLists = {
    "rtsp://192.169.1.53/stream1",
    "rtsp://192.169.1.53/stream1",
    "rtsp://192.169.1.53/stream1",
    "rtsp://192.169.1.53/stream1",
    "rtsp://192.169.1.53/stream1",
    "rtsp://192.169.1.53/stream1",
    "rtsp://192.169.1.53/stream1",
    "rtsp://192.169.1.53/stream1"
};

std::vector<std::string> videoLists = {
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4",
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4",
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4",
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4",
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4",
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4",
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4",
    "/home/vboxuser/hsp-aidemo/models/frtest.mp4"
};

int main(int argc, char* argv[]) {
    if (argc > 1) {
        std::string command = argv[1];
        if (command == "--add") {
            gstObject* gstObj = new gstObject("/home/vboxuser/hsp-aidemo/models/testFR.mp4", INPUT_TYPE::VIDEO, 0);
            gstObj->addDB(DB_IMAGE_PATH);
            return 0;
        } else if (command == "--remove") {
            return 0;
        } else {
            return 0;
        }
    } else {
        std::vector<gstObject*> gstObj(NUM_THREADS);
        for (int i = 0; i < NUM_THREADS; i++) {
            gstObj[i] = new gstObject(rtspLists[i], INPUT_TYPE::RTSP, i);
            gstObj[i]->loadDB(DB_PATH);
            gstObj[i]->startThread();
        }
        for (int i = 0; i < NUM_THREADS; i++) {
            gstObj[i]->joinThread();
        }
    }
    sleep(2000);
    return 0;
}