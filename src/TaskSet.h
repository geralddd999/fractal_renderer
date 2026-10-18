#pragma once
#include <condition_variable>
#include <mutex>

class ThreadPool;

/*
 * Defines an interface that allows to aggregate a set of tasks to be executed in parallel but to synchronize behaviours in the group.
 * This implementation allows for waiting synchronously for all tasks in the group to finish executing.
 */
class TaskSet{
  private:
    std::mutex m_;
    std::condition_variable cv_;
    int active_tasks_ = 0;

    friend class ThreadPool;

    void increment(){
      std::lock_guard<std::mutex> lock(m_);
      ++active_tasks_;
    }

    void decrement(){
      {
        std::lock_guard<std::mutex> lock(m_);
        --active_tasks_;
      }
      cv_.notify_all();
    }

  public:
    TaskSet() = default; //too lazy to do it myself.

    // avoid copy
    TaskSet(const TaskSet&) = delete;
    TaskSet& operator=(const TaskSet&) = delete;

    void wait(){
      std::unique_lock<std::mutex> lock(m_);
      cv_.wait(lock, [this](){
        return active_tasks_ == 0 ;
      });
    }
};
