#include <iostream>
#include <memory>
#include <unistd.h>
#include <mutex>
#include <thread>
#include "AsyncTasksPool.hpp"
#include "AsyncTask.hpp"

class MyTask : public AsyncTask<bool> {
public:
    MyTask(int x) : x_(x) {}

protected:
    bool run() override {
        for(int i = 2; i * i <= x_; i++) {
            if (x_ % i == 0) {
                return false;
            }
        }
        return true;
    }

private: 
    int x_;
};

int main() {
    AsyncTaskPool tasks_pool;

    int x = 42;

    std::unique_ptr<MyTask> task_ptr = std::make_unique<MyTask>(x);
    std::future<bool> future = task_ptr->get_future();
    tasks_pool.submit(std::move(task_ptr));

    // some code

    bool result = future.get();
    if (result == true) {
        std::cout << x << " is prime" << std::endl;
    } else {
        std::cout << x << " is not prime" << std::endl;
    }
}
