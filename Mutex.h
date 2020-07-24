#pragma once

#include <pthread.h>
#include <assert.h>

class MutexLock
{
public:
    MutexLock() {
        int ret = pthread_mutex_init(&mutex_, NULL);
        assert(ret == 0);
    }
    ~MutexLock() {
        int ret = pthread_mutex_destroy(&mutex_);
        assert(ret == 0);
    }

    void lock()
    {
        pthread_mutex_lock(&mutex_);
    }

    void unlock()
    {
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex()
    {
        return &mutex_;
    }

private:
    pthread_mutex_t mutex_;

};

//RAII
class MutexLockGuard{
public:
    explicit MutexLockGuard(MutexLock& mutex):
        mutex_(mutex)
    {
        mutex_.lock();
    }

    ~MutexLockGuard()
    {
        mutex_.unlock();
    }


private:
    MutexLock& mutex_;

};


//禁止这样的使用方法 MutexLockGuard(mutex);
#define MutexLockGuard(x) error "missing object name"
