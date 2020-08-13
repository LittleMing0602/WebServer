#include "AsyncLogging.h"
#include "LogFile.h"
#include "../timer/TimeStamp.h"

AsyncLogging::AsyncLogging(const string& basename, size_t rollSize, int flushInterval):
    flushInterval_(flushInterval),
    running_(false),
    basename_(basename),
    rollSize_(rollSize),
    thread_(std::bind(&AsyncLogging::threadFunc, this), "Logging"),
    latch_(1),
    mutex_(),
    cond_(mutex_),
    currentBuffer_(new Buffer),
    nextBuffer_(new Buffer),
    buffers_()
{
    currentBuffer_->bzero();
    nextBuffer_->bzero();
    buffers_.reserve(16);
}

void AsyncLogging::append(const char* logline, int len)
{
    MutexLockGuard lock(mutex_);
    if(currentBuffer_->unused() > len)
    {
        currentBuffer_->append(logline, len);
    }
    else
    {
        buffers_.push_back(std::move(currentBuffer_));
        assert(currentBuffer_ == nullptr);

        if(nextBuffer_)
        {
            currentBuffer_ = std::move(nextBuffer_);
        }
        else
        {
            currentBuffer_.reset(new Buffer);
        }

        currentBuffer_->append(logline, len);
        cond_.notify();
    }
}

void AsyncLogging::threadFunc()
{
    latch_.countDown();
    LogFile output(basename_, rollSize_, false);

    BufferPtr newBuffer1(new Buffer);
    BufferPtr newBuffer2(new Buffer);
    newBuffer1->bzero();
    newBuffer2->bzero();

    BufferVector buffersToWrite;
    buffersToWrite.reserve(16);
    while(running_)
    {
        {
            MutexLockGuard lock(mutex_);
            if(buffers_.empty())
            {
                cond_.waitForSeconds(flushInterval_); // 等待前端写满一个或多个buffer或者超时
            }
            buffers_.push_back(std::move(currentBuffer_)); // 将当前缓冲区移入buffers_
            currentBuffer_ = std::move(newBuffer1); // 将空闲的缓冲区置为当前缓冲区
            buffersToWrite.swap(buffers_); // 将buffers_交换出去，以便在临界区外访问buffersToWrite

            if(!nextBuffer_)
            {
                nextBuffer_ = std::move(newBuffer2); // 确保前端有一个空闲的buffer, 减少前端临界区的内存分配，从而减小临界区长度
            }
        }

        // 处理消息堆积
        // 前端陷入死循环，拼命发送日志消息，超过后端处理能力，这就是典型的生产速度超过消费速度问题，
        // 会造成数据在内存中堆积，严重时引发性能问题（内存不足）或程序崩溃（分配内存失败）
        if(buffersToWrite.size() > 25)
        {
            char buf[256];
            snprintf(buf, sizeof buf, "Dropped log messages at %s, %zd, larger buffers\n",
                     TimeStamp::now().toString().c_str(), buffersToWrite.size() - 2); 

            fputs(buf, stderr);
            output.append(buf, static_cast<int>(strlen(buf)));
            // 仅保留两块缓冲区，丢弃多余的日志，以腾出内存
            buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
        }

        for(size_t i = 0; i < buffersToWrite.size(); ++i)
        {
            output.append(buffersToWrite[i]->data(), buffersToWrite[i]->length());
        }

        if(buffersToWrite.size() > 2)
        {
            buffersToWrite.resize(2); // 仅保存两个buffer，用于newBuffer1和newBuffer2
        }
        
        // 使用buffersToWrite中的buffer来填充newBuffer1、2, 不会产生缺页异常，提高效率
        if(!newBuffer1)
        {
            newBuffer1 = std::move(buffersToWrite.back());
            newBuffer1->reset();
            buffersToWrite.pop_back();
        }
        if(!newBuffer2)
        {
            newBuffer2 = std::move(buffersToWrite.back());
            newBuffer2->reset();  // 清空buffer_
            buffersToWrite.pop_back();
        }

        buffersToWrite.clear();
        output.flush();
    }
    output.flush();
}
