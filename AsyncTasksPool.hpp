#ifndef ASYNC_TASK_POOL_H
#define ASYNC_TASK_POOL_H

#include <utility>
#include <future>
#include <functional>
#include <memory>
#include "ThreadGuard.hpp"
#include "ThreadSafeQueue.hpp"
#include "AsyncTask.hpp"

#define DEFAULT_COUNT_OF_WORKERS 8

class AsyncTaskPool {
public:
    AsyncTaskPool() {
        for (int i = 0; i < threadCount_; i++) {
            ThreadGuard([this](){
                while(true) {
                    std::unique_ptr<BaseTask> task_ptr(tasks_.pop());
                    task_ptr->operator()();
                }
            });
        }
    }

    AsyncTaskPool( const AsyncTaskPool& ) = delete;
    AsyncTaskPool& operator=( const AsyncTaskPool& ) = delete;

    void submit(std::unique_ptr<BaseTask> task_ptr) {

        tasks_.push(std::move(task_ptr));

    }

private:
    ThreadSafeQueue<std::unique_ptr<BaseTask>> tasks_;
    const int threadCount_ = DEFAULT_COUNT_OF_WORKERS;
};

#endif // ASYNC_TASK_POOL_H