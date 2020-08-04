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
    void join();
    bool started() const { return started_; }
    const std::string& name() const { return name_; }

private:
    static void* startThread(void* thread);
    void runInThread();

    pthread_t threadId_;
    pid_t tid_;
    ThreadFunc func_;
    bool started_;
    std::string name_;
};

#endif

