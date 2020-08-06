#ifndef LOG_STREAM_H
#define LOG_STREAM_H

#include <string>
#include <string.h>

using std::string;

const int kSmallBuffer = 4096;
const int kLargeBuffer = 4096 * 1024;

template<int SIZE>
class FixedBuffer
{
public:
    FixedBuffer(): cur_(data_)
    {}

    ~FixedBuffer()
    {}

    void append(const char* buf, size_t len)
    {
        if(static_cast<size_t>(unused()) > len)
        {
            memcpy(data_, buf, len);
            cur_ += len;
        }
    }

    const char* date() const { return data_; }

    int unused() const
    { return static_cast<int>(end() - cur_); }
    int length() const 
    { return static_cast<int>(cur_ - data_); }

    char* current() { return cur_; }

    void reset() { cur_ = data_; }

    void bzero() { ::bzero(data_, sizeof data_); }
    
    void retrieve(size_t len) { cur_ += len; }


private:
    const char* end() const { return data_ + sizeof data_; }

    char data_[SIZE];
    char* cur_; // 指向第一个空位置
};

class LogStream
{
public:
    typedef LogStream self;
    typedef FixedBuffer<kSmallBuffer> Buffer;

    LogStream() {}
    ~LogStream() {}

    self& operator<<(short);
    self& operator<<(unsigned short);
    self& operator<<(int);
    self& operator<<(unsigned int);
    self& operator<<(long);
    self& operator<<(unsigned long);
    self& operator<<(long long);
    self& operator<<(unsigned long long);

    self& operator<<(const void*);

    self& operator<<(float);
    self& operator<<(double);

    self& operator<<(char);

    self& operator<<(const char*);
    self& operator<<(const string&);

    void append(const char* data, int len);
    const Buffer& buffer() const { return buffer_; }
    void resetBuffer() { buffer_.reset(); }


private:
    template <typename T>
    void formatInteger(T v);

    Buffer buffer_;
    static const int kMaxNumericSize = 32;
};

// 格式化
class Fmt
{
public:
    template<typename T>
    Fmt(const char* fmt, T val)
    {
        length_ = snprintf(buf_, sizeof buf_, fmt, val);
    }

    const char* data() const { return buf_; }
    int length() const { return length_; }

private:
    char buf_[32];
    int length_;
};

inline LogStream& operator<<(LogStream& s, const Fmt& fmt)
{
    s.append(fmt.data(), fmt.length());
    return s;
}


#endif

