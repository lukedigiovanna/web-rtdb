#include "rtdb_threadpool.h"

namespace rtdb {

ThreadPool::ThreadPool(int n) {
    for (int i = 0; i < n; i++) {
        d_workers.emplace_back([this] {
            while (true) {
                Job job;
                {
                    std::unique_lock<std::mutex> _lock{d_workerLock};
                    // d_cv.wait(d_workerLock, [this] {
                    //     return !d_jobs.empty() || d_shuttingDown;
                    // });
                    d_cv.wait(d_workerLock);
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() { shutdown(); }

void ThreadPool::enqueueJob(Job job) {
    {
        std::unique_lock<std::mutex> _lock{d_workerLock};
        d_jobs.push(job);
        d_cv.notify_one();
    }
}

void ThreadPool::shutdown() {
    std::unique_lock<std::mutex> _lock{d_workerLock};
    d_shuttingDown = true;
    d_cv.notify_all();
}

} // namespace rtdb