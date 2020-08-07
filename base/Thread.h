#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <functional>

class Thread
{
public:
    typedef typename std::function<void()> ThreadFunc;
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();

    void start();
    void join();  // 调用pthread_join
    bool started() const { return started_; }
    const std::string& name() const { return name_; }

private:
    static void* startThread(void* thread);
    void runInThread();

    pthread_t threadId_;  // 线程标识符
    pid_t tid_;  // 线程号
    ThreadFunc func_;  // 线程执行函数
    bool started_;  // 线程是否开启
    std::string name_;  // 线程名
};

#endif

