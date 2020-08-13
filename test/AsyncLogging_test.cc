#include <iostream>
#include "../log/AsyncLogging.h"
#include "../log/Logging.h"

AsyncLogging* g_asyncLog = nullptr;

void output(const char* p, int len)
{
    g_asyncLog->append(p, len);
}

void bench()
{
    int cnt = 0;
    for(int t = 0; t < 30; ++t)
    {
        TimeStamp start = TimeStamp::now();
        for(int i = 0; i < 1000; ++i)
        {
            LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz " <<
                string(3000, 'X') << cnt;
            ++cnt;
        }
        TimeStamp end = TimeStamp::now();
        printf("%f\n", timeDifference(end, start) * 1000000 / 1000);
    }
}

int main()
{
    AsyncLogging log("ming", 500 * 1024 * 1024);
    log.start();
    g_asyncLog = &log;
    Logger::setOutput(output);
    bench();
    return 0;
}

