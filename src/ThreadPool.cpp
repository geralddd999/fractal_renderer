#include <ThreadPool.h>
#include <condition_variable>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

Worker::Worker(std::queue<std::function<void()>>& q, std::mutex& m, std::condition_variable& cv, std::size_t id)
: id_(id), mutex_(m), queue_(q), cv_(cv)
{
  thread_ = std::thread([this](){
    while(true){
      std::function<void()> task;
      {
        std::unique_lock<std::mutex> lock(this->mutex_);
        this->cv_.wait(lock, [this](){
          return !this->queue_.empty();
        });

        if(this->queue_.empty()) return;
        task = std::move(this->queue_.front());
        this->queue_.pop();
      }
      task();
    }
  });
}

Worker::~Worker(){
  if (thread_.joinable()){
    thread_.join();
  }
}

ThreadPool::ThreadPool(std::size_t threads_num){

  std::size_t t_num = std::min(static_cast<unsigned int>(threads_num), std::thread::hardware_concurrency());
  pool_.reserve(t_num);
  for(std::size_t i = 0 ; i < t_num ; ++i){
    pool_.emplace_back(std::make_unique<Worker>(tasks_queue_, lock_, work_available, i));
  }
}

ThreadPool::ThreadPool(){
  auto threads = std::thread::hardware_concurrency();
  pool_.reserve(threads/2);

  for(std::size_t i = 0 ; i < threads ; ++i){
      pool_.emplace_back(std::make_unique<Worker>(tasks_queue_, lock_, work_available, i));
    }
}

ThreadPool::~ThreadPool(){
  {
    std::unique_lock<std::mutex> lock(lock_);
    stop_ = true;
  }
  work_available.notify_all();
}
