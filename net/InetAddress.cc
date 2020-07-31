#include "InetAddress.h"
#include <string.h>

InetAddress::InetAddress(uint16_t port)
{
    bzero(&addr_, sizeof addr_);
    addr_.sin_family = AF_INET;
    addr_.sin_addr.s_addr = INADDR_ANY;
    addr_.sin_port = htons(port);
}

std::string InetAddress::toIp() const
{
    char buf[32];
    size_t size = sizeof buf;
    inet_ntop(AF_INET, &addr_.sin_addr, buf, static_cast<socklen_t>(size));
    return buf;
}

void InetAddress::setAddrInet(const struct sockaddr_in& addr)
{
    addr_ = addr;
}
