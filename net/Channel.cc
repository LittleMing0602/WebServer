#include "Channel.h"
#include <poll.h>
#include "EventLoop.h"

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

/*
*POLLHUP : 表示套接字不再连接，在TCP中意味着FIN已经被接受和发送
*POLLERR : 表示套接字出现异步错误，在TCP中意味着接受或发送了RST
* 处于上述两个条件的套接字都还处于打开状态，但可能已经调用了shutdown()。
* 
*POLLNVAL : 表示套接字文件描述符未打开，close()它会是一个错误。
*/

void Channel::handleEvent()
{
    if(revents_ & POLLNVAL)
    {
        // 输出日志        
    }
    if((revents_ & POLLHUP) && !(revents_ & POLLIN))  // POLLHUP对方描述符挂起
    {
        if(closeCallback_) closeCallback_();
    }
    if(revents_ & (POLLERR | POLLNVAL))  // POLLERR发生错误， POLLNVAL描述符不是一个打开的文件
    {
        if(errorCallback_) errorCallback_();
    }
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))  // POLLIN普通或优先级带数据可读，POLLPRI高优先级数据可读，POLLEDHUP
    {
        if(readCallback_) readCallback_();
    }
    if(revents_ & POLLOUT)  // 普通数据可写
    {
        if(writeCallback_) writeCallback_();
    }
}

//将channel添加到reactor中
void Channel::update()
{
    loop_->updateChannel(this);
}

// 调用这个函数之前确保调用disableAll
void Channel::remove()
{
    loop_->removeChannel(this);
}