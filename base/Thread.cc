#include "Thread.h"
#include "CurrentThread.h"
#include <assert.h>

Thread::Thread(const ThreadFunc& func, const std::string& name):
    threadId_(0),
    tid_(0),
    func_(func),
    started_(false),
    name_(name)
{
    
}

Thread::~Thread()
{
	
}

// 调用pthread_create，开启线程
void Thread::start()
{
    started_ = true;
    errno = pthread_create(&threadId_, NULL, startThread, this);
    if(errno != 0)
    {
        //LOG_SYSFATAL <<"Failed in pthread_create";
    }
}

// 调用pthread_join
void Thread::join()
{
    assert(started_);
    pthread_join(threadId_, NULL);
}

// pthread_create注册的线程函数，该函数调用runInThread
void* Thread::startThread(void* obj)
{
    Thread* thread = static_cast<Thread*>(obj);
    thread->runInThread();
    return NULL;
}

// 调用用户提供的函数func_
void Thread::runInThread()
{
    tid_ = tid(); // 将线程号保存到 t_cachedId, t_tidString中
    t_threadName = name_.c_str();  // 保存线程名
    func_();  
    t_threadName = "finished";
}


