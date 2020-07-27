#include <iostream>
#include "log/log.h"
#include <unistd.h>

void* startroute(void* arg)
{
    while(1)
    {
    	LOG_DEBUG("aaaa");
    	sleep(1);
    }
}

void* startroute1(void* arg)
{
    while(1)
    {
    	LOG_DEBUG("bbbb");
    	sleep(1);
    }
}

void* startroute2(void* arg)
{
    while(1)
    {
    	LOG_DEBUG("cccc");
    	sleep(1);
    }
}

void* startroute3(void* arg)
{
    while(1)
    {
    	LOG_DEBUG("dddd");
    	sleep(1);
    }
}

int main()
{
    Log* log = Log::getInstance();
    log->init("ming", 8192, 10, 4);

    LOG_DEBUG("hahahhahah");
    LOG_DEBUG("Heeeeeeeeeee");
    pthread_t a,b,c,d;
    pthread_create(&a, NULL, startroute, NULL);
    pthread_create(&b, NULL, startroute1, NULL);
    pthread_create(&c, NULL, startroute2, NULL);
    pthread_create(&d, NULL, startroute3, NULL);

    pthread_join(a,NULL);
    pthread_join(b,NULL);
    pthread_join(c,NULL);
    pthread_join(d,NULL);

    return 0;
}

