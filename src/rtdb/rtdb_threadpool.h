#ifndef RTDB_THREADPOOL_H
#define RTDB_THREADPOOL_H

#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace rtdb {

// Implements a basic work-sharing based thread pool.
class ThreadPool {
  public:
    using Job = std::function<void(void)>;

  protected:
    std::vector<std::thread> d_workers;
    std::queue<Job> d_jobs;
    std::mutex d_workerLock;
    std::condition_variable d_cv;
    bool d_shuttingDown;

  public:
    ThreadPool(int numThreads);
    ~ThreadPool();

    void enqueueJob(Job job);
    void shutdown();
};

} // namespace rtdb

#endif