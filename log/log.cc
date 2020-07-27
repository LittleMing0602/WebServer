#include "log.h"
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <string>
#include "../lock/BoundedBlockQueue.h"
#include <stdarg.h>

Log::Log()
{
    count_ = 0;
    isAsync = false;
}

int Log::init(const char* filename, int logBufSize, int maxLineNum, int maxQueueSize)
{
    if(maxQueueSize > 0)
    {
        isAsync = true;
        maxQueueSize_ = maxQueueSize; 
        logQueue_ = new BoundedBlockQueue<std::string>(maxQueueSize);
        pthread_t thread;
        pthread_create(&thread, NULL, asynLogThread, this);
    }

    logBufSize_ = logBufSize;
    logBuf_ = new char[logBufSize_];
    memset(logBuf_, '\0', logBufSize_);

    maxLineNum_ = maxLineNum;

    char logFullName[256] = {0};

    time_t t = time(NULL);
    struct tm* tmp = localtime(&t);
    struct tm timeStruct = *tmp;

    const char* p = strrchr(filename, '/');

    if(p == nullptr)
    {
    	strcpy(logName_, filename);
        snprintf(logFullName, 255, "%d_%02d_%02d_%s", timeStruct.tm_year + 1900,
                 timeStruct.tm_mon + 1, timeStruct.tm_mday, filename);
    }
    else
    {
        strcpy(logName_, p+1);
        strncpy(dirName_, filename, p - filename + 1);
        snprintf(logFullName, 255, "%s%d_%02d_%02d_%s", dirName_,  timeStruct.tm_year + 1900,
                 timeStruct.tm_mon + 1, timeStruct.tm_mday, logName_);
    }
    
    today_ = timeStruct.tm_mday;
    
    fp_ = fopen(logFullName, "a");
    if(fp_ == NULL)
    {
        return -1;
    }

    return 0;
}

void* Log::asynLogThread(void* arg)
{
    Log* log = static_cast<Log*>(arg);
    log->asyncWriteLog();
}

int Log::writeLog(int level, const char* format, ...)
{
    struct timeval tv = {0,0};
    gettimeofday(&tv, NULL);
    time_t t = tv.tv_sec;
    struct tm* ts = localtime(&t);

    const char* preFix = nullptr;
    
    switch(level)
    {
    case 0:
        preFix = "[debub]:";
        break;
    case 1:
        preFix = "[info]:";
        break;
    case 2:
        preFix = "[warn]:";
        break;
    case 3:
        preFix = "[erro]:";
        break;
    default:
        preFix = "[info]:";
        break;
    }


    {
        MutexLockGuard lock(mutex_);
        
        ++count_;


        // 如果要过了一天或者文件行数写满了
        if(today_ != ts->tm_mday || count_ % maxLineNum_ == 0)
        {
            //首先关闭原文件
            fflush(fp_);
            fclose(fp_);
            char newName[256] = {0};

            if(today_ != ts->tm_mday)
            {
                snprintf(newName, 255, "%s%d_%02d_%02d_%s", dirName_, ts->tm_year,
                         ts->tm_mon, ts->tm_mday, logName_);
                ++today_;
                ++count_;
            }
            else
            {
                snprintf(newName, 255, "%s%d_%02d_%02d_%s.%lld", dirName_, ts->tm_year + 1900, 
                         ts->tm_mon+1, ts->tm_mday, logName_, count_ / maxLineNum_);
            }

            fp_ = fopen(newName, "a");
        }
    }

    va_list valist;
    va_start(valist, format);

    std::string logStr;
    
    {
        MutexLockGuard lock(mutex_);
        int n = snprintf(logBuf_, 48, "%d-%02d-%02d %02d:%02d:%02d.%06ld %s ", 
                         ts->tm_year+1900, ts->tm_mon + 1, ts->tm_mday, ts->tm_hour,
                         ts->tm_min, ts->tm_sec, tv.tv_usec, preFix);
        int m = vsnprintf(logBuf_+n, logBufSize_ - 1, format, valist);

        logBuf_[n + m] = '\n';
        logBuf_[n + m + 1] = '\0';

        logStr = logBuf_;
    }

    if(isAsync && !logQueue_->full())
    {
        logQueue_->push(logStr);
    }
    else
    {
        MutexLockGuard lock(mutex_);
        fputs(logStr.c_str(), fp_);

    }

    va_end(valist);

}


void Log::flush()
{
    MutexLockGuard lock(mutex_);
    fflush(fp_);
}

void Log::asyncWriteLog()
{
    std::string log;
    while(1)
    {
        log = logQueue_->pop();
        MutexLockGuard lock(mutex_);
        fputs(log.c_str(), fp_);
    }
}
