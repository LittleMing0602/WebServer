#ifndef BOUNDED_BLOCK_QUEUE_H
#define BOUNDED_BLOCK_QUEUE_H

#include <deque>
#include "Mutex.h"
#include "Condition.h"
#include <assert.h>

template <class T>
class BoundedBlockQueue{
public:
    BoundedBlockQueue(int capacity):mutex_(), notEmpty_(mutex_), notFull_(mutex_)
    {
        capacity_ = capacity;
        circularBuffer_ = new T[capacity];
        size_ = 0;
        front_ = -1;
        back_ = -1;
    }

    ~BoundedBlockQueue()
    {
        MutexLockGuard lock(mutex_);
        if(circularBuffer_ != NULL)
        {
            delete[] circularBuffer_;
        }
    }

    void push(const T& t)
    {
        MutexLockGuard lock(mutex_);
        
        while(size_ >= capacity_)  // 这里不能直接调用full，否则造成死锁
        {
            notFull_.wait();
        }

        assert(!(size_ >= capacity_));

        back_ = (back_ + 1) % capacity_;
        circularBuffer_[back_] = t;
        
        ++size_;
        
        notFull_.notify();
        
    }

    T pop()  // 直接返回T会有性能问题吗
    {
        MutexLockGuard lock(mutex_);
        while(size_ == 0)  // 这里不能直接调用full()，否则会造成死锁
        {
            notEmpty_.wait();
        }
        
        assert(!(size_ == 0));

        front_ = (front_+1) % capacity_;
        T t = circularBuffer_[front_];
        --size_;

        notFull_.notify();

        return t;
    }

    bool full()
    {
        MutexLockGuard lock(mutex_);
        if(size_ >= capacity_)
        {
            return true;
        }

        return false;
    }

    bool empty()
    {
        MutexLockGuard lock(mutex_);
        if(size_ == 0)
        {
            return true;
        }

        return false;
    }

private:
    MutexLock mutex_;
    Condition notEmpty_;
    Condition notFull_;
    T* circularBuffer_; // 循环数组
    int size_;
    int capacity_;
    int front_; // 始终指向数据的前一个空位
    int back_;  // 始终指向最后一个数据
};

#endif

