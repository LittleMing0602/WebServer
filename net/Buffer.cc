#include "Buffer.h"
#include <sys/uio.h>

const char Buffer::kCRLF[] = "\r\n";

// 使用scatter/gather IO技术，一部分缓冲区取自栈上，这样输入缓冲区足够大，
// 通常一次readv就可以读完，减少系统调用次数
ssize_t Buffer::readFd(int fd, int* savedErrno)
{
    char extraBuf[65535];
    struct iovec vec[2];
    const size_t writable = writableBytes();

    vec[0].iov_base = begin() + readIndex_;
    vec[0].iov_len = writable;

    vec[1].iov_base = extraBuf;
    vec[1].iov_len = sizeof extraBuf;

    const ssize_t n = readv(fd, vec, 2);
    if(n < 0)
    {
        *savedErrno = errno;
    }
    else if(static_cast<size_t>(n) <= writable)
    {
        writeIndex_ += n;
    }
    else
    {
        writeIndex_ = buffer_.size();
        append(extraBuf, n - writable);
    }
    
    return n;
}
