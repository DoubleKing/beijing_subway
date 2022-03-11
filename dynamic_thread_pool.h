/*
	动态线程池代码参考grpc
*/


#ifndef __DYNAMIC_THREAD_POOL_H__
#define __DYNAMIC_THREAD_POOL_H__

#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>


class DynamicThreadPool {
 public:
  explicit DynamicThreadPool(int reserve_threads);
  ~DynamicThreadPool();

  void Add(const std::function<void()>& callback);

 private:
  class DynamicThread {
   public:
    DynamicThread(DynamicThreadPool* pool);
    ~DynamicThread();

   private:
    DynamicThreadPool* pool_;
    std::thread thd_;
    void ThreadFunc();
  };
  std::mutex mu_;
  std::condition_variable cv_;
  std::condition_variable shutdown_cv_;
  bool shutdown_;
  std::queue<std::function<void()>> callbacks_;
  int reserve_threads_;
  int nthreads_;
  int threads_waiting_;
  std::list<DynamicThread*> dead_threads_;

  void ThreadFunc();
  static void ReapThreads(std::list<DynamicThread*>* tlist);
};


#endif
