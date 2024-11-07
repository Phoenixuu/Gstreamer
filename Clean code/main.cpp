#include"rtsp_decode.hpp"
#include <X11/Xlib.h>
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    std::cout << "Starting program..." << std::endl;
    XInitThreads();

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <RTSP URI>\n";
        return -1;
    }

    VideoDecoder decoder(argv[1]);
    GMainLoop *loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);

    return 0;
}
