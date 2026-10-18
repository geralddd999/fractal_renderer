#pragma once

#include "TaskSet.h"
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

/*
 * Represents each thread present in the `ThreadPool`, each worker will execute tasks from the Task queue
 */
struct Worker{
  std::thread thread_;
  std::size_t id_;
  std::mutex& mutex_;
  std::queue<std::function<void()>>& queue_;
  std::condition_variable& cv_;
  Worker(std::queue<std::function<void()>>& q, std::mutex& m, std::condition_variable& cv, std::size_t id);
  ~Worker();
  //avoid copy!!!
  Worker(const Worker&) = delete;
  Worker& operator=(const Worker&) = delete;
};

/*
 * Provides an abstraction to create a "pool" of threads for concurrent execution of tasks
 */
class ThreadPool{
  private:
    std::vector<std::unique_ptr<Worker>> pool_;
    std::queue<std::function<void()>> tasks_queue_;
    std::condition_variable work_available;
    std::mutex lock_;
    int remaining_;
    size_t thread_num_;
    bool stop_ = false; //TODO: actually use the flag to stop the threads if not they run forveery

  public:
    ThreadPool();
    ThreadPool(std::size_t thread_num);
    ~ThreadPool();
    template<typename F, typename... Args>
    void execute(F&& f, Args&&... args);

    template<typename F, typename... Args>
    void execute_par(TaskSet& tset, F&& f, Args&&... args);
    size_t size(){ return pool_.size();}
};


template<typename F, typename... Args>
void ThreadPool::execute(F&& f, Args&&... args){
    {
      std::unique_lock<std::mutex> lock(lock_);

      tasks_queue_.emplace(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
    }
    work_available.notify_one();
}


template<typename F, typename... Args>
void ThreadPool::execute_par(TaskSet& tset, F&& f, Args&&... args){

  tset.increment();
  {
    std::unique_lock<std::mutex> lock(lock_);

    tasks_queue_.emplace([
      task = std::bind(std::forward<F>(f), std::forward<Args>(args)...),
      &tset]() mutable{
        task();
        tset.decrement();
      });
  }
  work_available.notify_one();
}
