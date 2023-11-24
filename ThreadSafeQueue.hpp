#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#define DEFAULT_CAPACITY 20

template <typename T>
class ThreadSafeQueue {
    typedef T type_name;

public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue( const ThreadSafeQueue& ) = delete;
    ThreadSafeQueue& operator=( const ThreadSafeQueue& ) = delete;

    void push(type_name&& value) {
        std::unique_lock<std::mutex> lock(mutex_);
        isNotQueueFull_.wait(lock, [this](){ return queue_.size() < capacity_; });

        queue_.push(std::forward<type_name>(value));

        isNotQueueEmpty_.notify_all();
    }

    type_name pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        isNotQueueEmpty_.wait(lock, [this](){ return !queue_.empty(); });
        type_name value(std::move(queue_.front()));
    
        queue_.pop();
        isNotQueueFull_.notify_all();

        return value;
    }

private:
    std::queue<type_name> queue_;
    std::mutex mutex_;
    std::condition_variable isNotQueueEmpty_;
    std::condition_variable isNotQueueFull_;
    const int capacity_ = DEFAULT_CAPACITY;
};

#endif // THREAD_SAFE_QUEUE_H