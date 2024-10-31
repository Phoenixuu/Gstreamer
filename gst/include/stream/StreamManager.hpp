#ifndef StreamManager_hpp
#define StreamManager_hpp

#include <memory>
#include <string>
#include <map>
#include <mutex>
#include <unordered_map>

#include "Stream.hpp"

class StreamManager
{
    public:
        static StreamManager& GetInstance();
        std::shared_ptr<Stream> getStream(std::string strUrl);

        void clean();

        // std::unordered_map<int, std::array<int, 4>> umpGrid4 = {
        //     {1, {0, 0, 960, 540}},
        //     {2, {960, 0, 960, 540}},
        //     {3, {0, 540, 960, 540}},
        //     {4, {960, 540, 960, 540}}
        // };

        std::unordered_map<int, std::array<int, 4>> umpGrid4 = {
            {1, {0, 0, 960, 540}},
            {2, {960, 0, 1280, 720}},
            {3, {0, 540, 640, 360}},
            {4, {960, 540, 960, 540}}
        };

    private:
        std::map<std::string, std::shared_ptr<Stream>> m_mpStreams;

        std::mutex m_mtLock;

        int m_iNumGrid = 0;

};

#endif // StreamManager_hpp