#ifndef COUNT_DOWN_LATCH_H
#define COUNT_DOWN_LATCH_H

#include "Mutex.h"
#include "Condition.h"

class CountDownLatch
{
public:
    explicit CountDownLatch(int count);
    
    void wait();

    void countDown();

    int getCount() const;

private:
    mutable MutexLock mutex_; // 因为getCount中要对mutex_进行写操作，所以设为mutable
    Condition cond_;
    int count_;
};

#endif
