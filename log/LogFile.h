#ifndef LOG_FILE_H
#define LOG_FILE_H

#include <memory>
#include <string>
#include "../lock/Mutex.h"

using std::string;

class LogFile
{
public:
    LogFile(const string& basename, size_t rollSize, bool threadSafe = true, int flushInterval = 3);
    ~LogFile();
    void append(const char* logline, int len);
    void flush();


private:
    void appendUnlocked(const char* logLine, int len);
    string getLogFileName(const string& basename, time_t* now);
    void rollFile();

    const string basename_;
    const size_t rollSize_;
    const int flushInterval_;
    int count_;

    std::unique_ptr<MutexLock> mutex_;

    time_t startOfPeriod_;
    time_t lastRoll_;
    time_t lastFlush_;

    class File;
    std::unique_ptr<File> file_;

    const static int kCheckTimeRoll_ = 1024; // 每写1024条日志检查是否需要滚动日志文件或刷新日志文件
    const static int kRollPerSeconds_ = 60 * 60 * 24; // 每天滚动一次日志
};

#endif
