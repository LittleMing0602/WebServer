#ifndef CALLBACK_H
#define CALLBACK_H

#include <memory>
#include "InetAddress.h"

class TcpConnection;
class InetAddres;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr; 
typedef std::function<void()> EventCallback;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef std::function<void (const TcpConnectionPtr&, const char*, ssize_t)> MessageCallback;
typedef std::function<void(int, InetAddress& addr)> NewConnectionCallback;

#endif


