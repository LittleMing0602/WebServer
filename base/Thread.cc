#include "Thread.h"

Thread::Thread(const ThreadFunc& func):
    func_(func)
{

}

Thread::~Thread()
{
	
}

void Thread::start()
{
    pthread_create(&threadId_, NULL, startThread, this);
}

void Thread::join()
{
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
    func_();
}


