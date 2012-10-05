#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "udp.h"

CUdp::CUdp(int family, in_port_t port, const char *ip) : 
    CSocket(family, SOCK_DGRAM, 0)
{
    memset(&m_addrIn, 0, sizeof(m_addrIn));
    m_addrIn.sin_family = family;
    m_addrIn.sin_port = htons(port);

    if(ip)
        inet_aton(ip, &m_addrIn.sin_addr);
    else 
        m_addrIn.sin_addr.s_addr = htonl(INADDR_ANY);
}

CUdp::CUdp(struct sockaddr_in &addrIn) :
    CSocket(addrIn.sin_family , SOCK_DGRAM, 0)
{
    memcpy(&m_addrIn, &addrIn, sizeof(m_addrIn));
}

void CUdp::setAddrIn(unsigned short port, int family,const char *ip)
{
    memset(&m_addrIn, 0, sizeof(m_addrIn));
    m_addrIn.sin_family = family;
    m_addrIn.sin_port = htons(port);

    if(ip) 
        inet_aton(ip, &m_addrIn.sin_addr);
    else
        m_addrIn.sin_addr.s_addr = htonl(INADDR_ANY);
}

void CUdp::setAddrIn(struct socckaddr_in &addrIn)
{
    memcpy(&m_addrIn, &addrIn, sizeof(m_addrIn));
}


int CUdp::Bind(void)
{
    int ret;
    
    if((ret = bind(m_sockfd,(struct sockaddr *) &m_addrIn, sizeof(m_addrIn))) < 0)
        fprintf(stderr, "[File:%s][%s][Line:%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return ret;
}

ssize_t CUdp::Send(const void *buf, size_t len, int flags)
{
    return send(m_sockfd, buf, len, flags);
}

ssize_t CUdp::SendTo(const void *buf, size_t len, 
                     const struct sockaddr_in &destAddr, int flags)
{
    return sendto(m_sockfd, buf, len, flags,
                  (struct sockaddr *)&destAddr, 
                  sizeof(destAddr));
}

ssize_t CUdp::SendMsg(const struct msghdr &msg, int flags)
{
    return sendmsg(m_sockfd, &msg, flags);
}

ssize_t CUdp::Recv(void *buf, size_t len, int flags)
{
    return recv(m_sockfd, buf, len, flags);
}

ssize_t CUdp::RecvFrom(void *buf, size_t len, 
                       struct sockaddr_in &srcAddr,
                       socklen_t &addLen, int flags)
{
    return recvfrom(m_sockfd, buf, len, flags, 
                    (struct sockaddr *)&srcAddr,
                    &addLen);
}

ssize_t CUdp::RecvMsg(struct msghdr &msg, int flags)
{
    return recvmsg(m_sockfd, &msg, flags);
}
