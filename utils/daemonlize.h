#ifndef DAEMONIZE_H
#define DAEMONIZE_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>


bool daemonlize()
{
    pid_t pid = fork();
    if(pid < 0)
    {
        return false;
    }
    else if(pid > 0)
    {
        exit(0);  // 父进程退出
    }
    
    //设置文件权限 
    umask(0);

    //创建新的绘画，设置本晋城为进程组的首领
    pid_t sid = setsid();

    if(sid < 0)
    {
        return false;
    }

    //切换工作目录
    if((chdir("/") < 0))
    {
        return false;
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    //还要关闭其他打开的文件描述符，代码省略
    open("/dev/null", O_RDONLY);
    open("/dev/null", O_RDWR);
    open("/dev/null", O_RDWR);
    return true;
}

#endif
