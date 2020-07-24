#pragma once

#include <deque>
#include "Mutex.h"
#include "Condition.h"


template <class T>
class BlockQueue
{
public:
    BlockQueue():mutex_(), notEmpty_(mutex_), queue_() {}
    ~BlockQueue() {}
    
    void push(const T& t)
    {
        MutexLockGuard lock(mutex_);
        queue_.push_back(t);
        notEmpty_.notify();
    }

    T take()
    {
        MutexLockGuard lock(mutex_);
        while(queue_.empty())
        {
            notEmpty_.wait();
        }

        T t = queue_.pop_front();
        return t;
    }


private:
    std::deque<T> queue_;    
    MutexLock mutex_;
    Condition notEmpty_;
};

