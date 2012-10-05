#ifndef _NETWORKING_UDP_H_
#define _NETWORKING_UDP_H_
#include "socket.h"
#include <sys/types.h>
#include <netinet/in.h>

class CUdp : public CSocket
{
protected:
    struct sockaddr_in  m_addrIn;

public:
    CUdp(int family, in_port_t port, const char *ip = NULL); 
        
    CUdp(struct sockaddr_in &addrIn);
    ~CUdp(){};

    void setAddrIn(unsigned short port, int family = AF_INET,
                   const char *ip = NULL);
    void setAddrIn(struct socckaddr_in &addrIn);
    
    int Bind(void);
    ssize_t Send(const void *buf, size_t len, int flags = 0);
    ssize_t SendTo(const void *buf, size_t len,
                   const struct sockaddr_in &destAddr, int flags = 0);
    ssize_t SendMsg(const struct msghdr &msg, int flags = 0);

    ssize_t Recv(void *buf, size_t len, int flags = 0);
    ssize_t RecvFrom(void *buf, size_t len, 
                     struct sockaddr_in &srcAddr,
                     socklen_t &addLen, int flags = 0);
    ssize_t RecvMsg(struct msghdr &msg, int flags = 0);
};
#endif
