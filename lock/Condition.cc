#include "Condition.h"
#include <errno.h>

bool Condition::waitForSeconds(int seconds)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    t.tv_sec += seconds;
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, mutex_.getPthreadMutex(), &t); 
}
