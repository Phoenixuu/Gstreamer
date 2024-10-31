#ifndef BlockingQueue_hpp
#define BlockingQueue_hpp

#include <queue>
#include <mutex>
#include <chrono>
#include <condition_variable>

template <typename T>
class BlockingQueue
{
    public:
        void push(T const &data) {
            {
                std::lock_guard<std::mutex> lock(this->guard);
                this->queue.push(data);
            }
            this->signal.notify_one();
        }

        bool isEmpty() const {
            std::lock_guard<std::mutex> lock(this->guard);
            return this->queue.empty();
        }

        unsigned int getSize() const {
            std::lock_guard<std::mutex> lock(this->guard);
            return this->queue.size();
        }

        #define QUEUE_BLOCK_POP(value) \
            value = this->queue.front(); \
            this->queue.pop()

        bool tryPop(T &value) {
            std::lock_guard<std::mutex> lock(this->guard);
            if (this->queue.empty()) {
                return false;
            }

            QUEUE_BLOCK_POP(value);
            return true;
        }

        void waitAndPop(T &value) {
            std::unique_lock<std::mutex> lock(this->guard);
            while (this->queue.empty()) {
                signal.wait(lock);
            }
            QUEUE_BLOCK_POP(value);
        }

        bool tryWaitAndPop(T &value, int waitTime) {
            std::unique_lock<std::mutex> lock(this->guard);
            if (this->queue.empty()) {
                signal.wait_for(lock, std::chrono::milliseconds(waitTime));
            }
            if (this->queue.empty()) {
                return false;
            }
            QUEUE_BLOCK_POP(value);
            return true;
        }

    private:

        std::queue<T> queue;
        mutable std::mutex guard;
        std::condition_variable signal;
};

#endif // BlockingQueue_hpp