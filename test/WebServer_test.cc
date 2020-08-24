#include "../server/WebServer.h"
#include "../net/InetAddress.h"
#include "../net/EventLoop.h"

int main()
{
    WebServer server(InetAddress(8000), 5);
    server.start();
}


