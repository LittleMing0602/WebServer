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

void Thread::start()
{
    started_ = true;
    errno = pthread_create(&threadId_, NULL, startThread, this);
    if(errno != 0)
    {
        //LOG_SYSFATAL <<"Failed in pthread_create";
    }
}

void Thread::join()
{
    assert(started_);
    pthread_join(threadId_, NULL);
}

void* Thread::startThread(void* obj)
{
    Thread* thread = static_cast<Thread*>(obj);
    thread->runInThread();
    return NULL;
}

void Thread::runInThread()
{
    tid_ = tid();
    t_threadName = name_.c_str();
    func_();
    t_threadName = "finished";
    
}


