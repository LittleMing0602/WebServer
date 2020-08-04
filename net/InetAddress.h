#ifndef  INETADRESS_H
#define INETADRESS_H

#include <arpa/inet.h>
#include <string>

class InetAddress
{
public:
    explicit InetAddress(uint16_t port);
    //InetAdress(const char* ip, uint16_t port);
    InetAddress(const struct sockaddr_in& addr):
        addr_(addr)
    {}
    ~InetAddress() {}

    std::string toIp() const;
    std::string toIpPort() const;
    const struct sockaddr_in& getAddrInet() const { return addr_; }
    void setAddrInet(const struct sockaddr_in& addr);
    
private:
    struct sockaddr_in addr_;

}

#endif
;

