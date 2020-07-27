#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include "../lock/Mutex.h"
#include "../lock/BoundedBlockQueue.h"
#include <string>


class Log
{
private:

    Log();
    Log(const Log&){}
public:
    ~Log() {}
    static Log*  getInstance()
    {
        static Log log;
        return &log;
        
    }

    int init(const char* filename, int logBufSize = 8192, int maxLineNum = 5000000, int maxQueueSize = 0);
    int writeLog(int level, const char* format, ...);
	void flush();
	
private:
    static void* asynLogThread(void* arg);
    void asyncWriteLog();
    

private:
    char dirName_[128];  //日志路径名
    char logName_[128];  //日志文件名
    int maxLineNum_;    // 一个日志文件最多写多少行
    long long count_;  // 当前写了多少行
    int logBufSize_;  // 日志缓存区的大小
    int today_;  // 日志当前在哪一天
    char* logBuf_;  // 日志缓冲区
    FILE* fp_;
    MutexLock mutex_;

    int maxQueueSize_;
    static Log* log_;

    BoundedBlockQueue<std::string>* logQueue_;  // 异步日志缓冲 
    
    bool isAsync;

};

#define LOG_DEBUG(format, ...) {Log::getInstance()->writeLog(0, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_INFO(format, ...) {Log::getInstance()->writeLog(1, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_WARN(format, ...) {Log::getInstance()->writeLog(2, format, ##__VA_ARGS__); Log::getInstance()->flush();}
#define LOG_ERROR(format, ...) {Log::getInstance()->writeLog(3, format, ##__VA_ARGS__); Log::getInstance()->flush();}

#endif

