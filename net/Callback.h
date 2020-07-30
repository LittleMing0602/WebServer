#ifndef CALLBACK_H
#define CALLBACK_H

#include <memory>

class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr; 

typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&, char*, ssize_t)> MessageCallback;

#endif


