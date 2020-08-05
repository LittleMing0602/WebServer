#ifndef BUFFER_H
#define BUFFER_H

#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <string>
#include <algorithm>

class Buffer
{
public:
    static const size_t kCheapPrePend = 8;
    static const size_t kInitialSize = 1024;

    Buffer(): 
        readIndex_(kCheapPrePend),
        writeIndex_(kCheapPrePend),
        buffer_(kCheapPrePend + kInitialSize)
    {}

    ~Buffer() {}
    
    size_t readableBytes() const
    { return writeIndex_ - readIndex_; }

    size_t writableBytes() const
    { return buffer_.size() - writeIndex_; }

    size_t prependableBytes() const
    { return readIndex_; }

    // 当前读到的位置
    const char* peek() const
    { return begin() + readIndex_; }


    void retrieve(int len)
    {
        assert(len <= readableBytes());
        if(len < readableBytes())
        {
            readIndex_ += len;
        }
        else
        {
            retrieveAll();
        }
    }

    void retrieveAll()
    {
        readIndex_ = kCheapPrePend;
        writeIndex_ = kCheapPrePend;
    }

    void retrieveUntil(const char* end)
    {
        assert(peek() < end);
        assert(end <= begin() + writeIndex_);
        retrieve(end - peek());
    }

    // 以string格式读出len长度的数据
    std::string retrieveAsString(size_t len)
    {
        std::string result(peek(), len);
        retrieve(len);
        return result;
    }   
    
    // 以string格式读出全部数据
    std::string retrieveAllAsString()
    {
        return retrieveAsString(readableBytes());
    }

    void append(const char* data, size_t len)
    {
        ensureWritableBytes(len);
        std::copy(data, data + len, begin() + writeIndex_); 
        writeIndex_ += len;
    }

    void append(const void* data, size_t len)
    {
        append(static_cast<const char*>(data), len);
    }

    void append(const std::string& str)
    {
        append(str.c_str(), str.size());
    }
    
    // 确保writable大于等于len, 否则分配空间到足够大小
    void ensureWritableBytes(size_t len)
    {
        if(writableBytes() < len)
        {
            makeSpace(len);
        }
        assert(writableBytes() >= len);
    }

    void prepend(const void* data, size_t len)
    {
        assert(len <= prependableBytes());
        readIndex_ -= len;
        const char* tmpd = static_cast<const char*>(data);
        std::copy(tmpd, tmpd + len, begin() +  readIndex_);
    }

    void swap(Buffer& buffer)
    {
        buffer_.swap(buffer.buffer_);
        std::swap(readIndex_, buffer.readIndex_);
        std::swap(writeIndex_, buffer.writeIndex_);
    }

    ssize_t readFd(int fd, int* savedErrno);

    const char* findCRLF() const
    {
        const char* crlf = std::search(peek(), begin() + writeIndex_, kCRLF, kCRLF + 2);
        return crlf == begin() + writeIndex_ ? NULL : crlf;
    }

    const char* findCRLF(const char* start) const
    {
        assert(peek() <= start);
        assert(start <= begin() + writeIndex_);
        const char* crlf = std::search(start, begin() + writeIndex_, kCRLF, kCRLF + 2);
        return crlf == begin() + writeIndex_ ? NULL : crlf;
    }

private:
    char* begin() { return &*buffer_.begin(); }
    const char* begin() const { return &*buffer_.begin(); }

    void makeSpace(size_t len)
    {
        // 如果prependable和writable的大小都不够，则分配空间
        if(writableBytes() + prependableBytes() < len + kCheapPrePend)
        {
            buffer_.resize(writeIndex_ + len);
        }
        else  // 否则，将readable的数据移到kCheapPrepend处, 就不用分配空间，节省了分配空间的时间
        {   
            assert(kCheapPrePend < readIndex_);
            size_t readable = readableBytes();
            std::copy(begin() + readIndex_, begin() + writeIndex_, begin() + kCheapPrePend);
            readIndex_ = kCheapPrePend;
            writeIndex_ = readIndex_ + readable;
        }
    }

    size_t readIndex_;
    size_t writeIndex_;
    std::vector<char> buffer_;
    static const char kCRLF[];
};

#endif
