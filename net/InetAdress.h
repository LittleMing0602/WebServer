#ifndef  INETADRESS_H
#define INETADRESS_H

#include <arpa/inet.h>
#include <string>

class InetAdress
{
public:
    explicit InetAdress(uint16_t port);
    //InetAdress(const char* ip, uint16_t port);
    ~InetAdress() {}

    std::string toIp() const;
    const struct sockaddr_in& getAddrInet() const { return addr_; }
    void setAddrInet(const struct sockaddr_in& addr);
    
private:
    struct sockaddr_in addr_;

}

#endif
;

