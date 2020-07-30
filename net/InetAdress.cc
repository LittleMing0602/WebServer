#include "InetAdress.h"
#include <string.h>

InetAdress::InetAdress(uint16_t port)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = INADDR_ANY;
    addr_.sin_port = htons(port);
}

std::string InetAdress::toIp() const
{
    char buf[32];
    size_t size = sizeof buf;
    inet_ntop(AF_INET, &addr_.sin_addr, buf, static_cast<socklen_t>(size));
    return buf;
}

void InetAdress::setAddrInet(const struct sockaddr_in& addr)
{
    addr_ = addr;
}
