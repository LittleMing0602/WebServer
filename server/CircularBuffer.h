#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <vector>
#include "../lock/Mutex.h"

template<class T>
class CircularBuffer
{
public:
    CircularBuffer(int capacity):
     capacity_(capacity),
     size_(0),
     end_(0),
     data_(),
     mutex_()
     {
        data_.resize(capacity);
     }
    ~CircularBuffer() {}
    
    void push(T t)
    {
        MutexLockGuard lock(mutex_);
        data_[end_] = t;
        if(size_ < capacity_)
        {
            ++size_;
        }
        ++end_;
        if(end_ >= capacity_)
        {
            end_ = 0;
        }
    }
    
    int size() const
    {
        return size_;
    }
    
    T& back() const
    {
        int backIndex = end_ - 1;
        if(backIndex < 0)
        {
            backIndex = capacity_ - 1;
        }
        
        return data_[backIndex];
    }

private:
    int capacity_;
    int size_;
    int end_;
    std::vector<T> data_;
    MutexLock mutex_;
};

#endif

