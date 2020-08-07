#include "Logging.h"
#include <assert.h>
#include "../base/CurrentThread.h"

__thread char t_errnobuf[512];
__thread char t_time[32];
__thread time_t t_lastSecond;

const char* strerror_tl(int savedErrno)
{
    return strerror_r(savedErrno, t_errnobuf, sizeof t_errnobuf);
}

Logger::LogLevel initLogLevel()
{
    return Logger::TRACE;
}

Logger::LogLevel g_logLevel = initLogLevel();

const char* LogLevelName[Logger::NUM_LOG_LEVELS] =
{
    "TRACE ",
    "DEBUG ",
    "INFO  ",
    "WARN  ",
    "ERROR ",
    "FATAL ",
};

// 在编译器获得字符串长度的辅助类
class T
{
public:
    T(const char* str, unsigned len):
        str_(str),
        len_(len)
    {
        assert(strlen(str) == len);
    }

    const char* str_;
    const unsigned len_;
};

inline LogStream& operator<<(LogStream& s, T v)
{
    s.append(v.str_, v.len_);
    return s;
}

inline LogStream& operator<<(LogStream& s, const Logger::SourceFile& v)
{
    s.append(v.data_, v.size_);
    return s;
}

void defaultOutPut(const char* msg, int len)
{
    size_t n = fwrite(msg, 1, len, stdout); // 将msg写入stdout
    (void)n;
}


void defaultFlush()
{
    fflush(stdout);
}

Logger::OutputFunc g_output = defaultOutPut;
Logger::FlushFunc g_flush = defaultFlush;


Logger::Impl::Impl(LogLevel level, int savedErrno, const SourceFile& file, int line):
    time_(TimeStamp::now()),
    stream_(),
    level_(level),
    line_(line),
    basename_(file)
{
    formatTime();
    tid();
    stream_ << T(tidString(), 6);
    stream_ << T(LogLevelName[level], 6);
    if(savedErrno != 0)
    {
        stream_ << strerror_tl(savedErrno) << " (errno=" << savedErrno << ") ";
    }

}

// 将时间进行格式化，添加到stream_中去 如20200807 15:24:30.123456
void Logger::Impl::formatTime()
{
    int64_t microSecondsSinceEpoch = time_.microSeconds();
    time_t seconds = static_cast<time_t>(microSecondsSinceEpoch / 1000000);
    int microSeconds = static_cast<int>(microSecondsSinceEpoch % 1000000);

    if(seconds != t_lastSecond)
    {
        t_lastSecond = seconds;
        struct tm tm_time;
        gmtime_r(&seconds, &tm_time);

        int len = snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d", 
                           tm_time.tm_year + 1900, tm_time.tm_mon + 1,
                           tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min,
                           tm_time.tm_sec);
        assert(len == 17); (void)len;
    }

    Fmt us(".%06dZ ", microSeconds);

    stream_ << T(t_time, 17) << T(us.data(), 9);
}


// 在末尾加上文件名和行号
void Logger::Impl::finish()
{
    stream_ << " - " << basename_ << ':' << line_ << '\n';
}

Logger::Logger(SourceFile file, int line):
    impl_(INFO, 0, file, line)
{

}

Logger::Logger(SourceFile file, int line, LogLevel level, const char* func):
    impl_(level, 0, file, line)
{
    impl_.stream_ << func << ' ';
}

Logger::Logger(SourceFile file, int line, LogLevel level):
    impl_(level, 0, file, line)
{
}

Logger::Logger(SourceFile file, int line, bool toAbort):
    impl_(toAbort ? FATAL:ERROR, errno, file, line)
{
}

Logger::~Logger()
{
    impl_.finish();
    const LogStream::Buffer& buf(stream().buffer());
    g_output(buf.data(), buf.length()); // 将buf的数据写入stdout中
    if(impl_.level_ == FATAL)
    {
        g_flush(); // 刷新stdout
        abort();
    }
}

void Logger::setLogLevel(Logger::LogLevel level)
{
    g_logLevel = level;
}

void Logger::setOutput(OutputFunc out)
{
    g_output = out;
}


void Logger::setFlush(FlushFunc flush)
{
    g_flush = flush;
}
