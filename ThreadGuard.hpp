#ifndef THREAD_GUARD_H
#define THREAD_GUARD_H

#include <thread>

class ThreadGuard {
public:

    template <typename Function, typename... Args>
    ThreadGuard(Function&& f, Args&& ...args) : thread_(std::forward<Function>(f), std::forward<Args>(args)...), is_joinable_(true) {}

    ThreadGuard(ThreadGuard&& other) {
        thread_ = std::move(other.thread_);
        is_joinable_ = true;
        other.is_joinable_ = false;
    }

    ThreadGuard& operator=(ThreadGuard&& other) {
        thread_ = std::move(other.thread_);
        is_joinable_ = true;
        other.is_joinable_ = false;
        return *this;
    }

    ThreadGuard( const ThreadGuard& ) = delete;
    ThreadGuard& operator=( const ThreadGuard& ) = delete;

    ~ThreadGuard() {
        if (is_joinable_ == true) {
            thread_.detach();
        }
    }

private:
    std::thread thread_;
    bool is_joinable_;
};

#endif // THREAD_GUARD_H
