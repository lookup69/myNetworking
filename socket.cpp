#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "socket.h"

using namespace lookup69;

Socket::Socket(int sd) : m_socket(sd) 
{
}

Socket::~Socket(void)
{
    this->close();
}

bool Socket::isSocketOpen(void)
{
    if(m_socket == -1)
        return false;

    return true;
}

int Socket::getAddrInfo(addressInfo_t &addressInfo)
{
    struct sockaddr_storage name;
    socklen_t               socklen = sizeof(name);
    int                     ret;

    memset(&name, 0, sizeof(name));
    if((ret = getpeername(m_socket, (struct sockaddr *)&name, &socklen)) < 0) 
        return ret;

    addressInfo.family = ((struct sockaddr *)&name)->sa_family;
    if(addressInfo.family == AF_INET) {
        struct sockaddr_in *addrIn = (struct sockaddr_in *) &name;

        addressInfo.port = ntohs(addrIn->sin_port);
        addressInfo.ip = inet_ntoa(addrIn->sin_addr);
    } else if(addressInfo.family == AF_INET6) {
        struct sockaddr_in6 *addrIn6 = (struct sockaddr_in6 *) &name;
        char                 ipTmp[512] = {0};

        addressInfo.port = ntohs(addrIn6->sin6_port);
        inet_ntop(AF_INET6, &addrIn6->sin6_addr, ipTmp, sizeof(ipTmp));
        addressInfo.ip = ipTmp;
    } else {
        ret = -1;
    }

    return ret;
}

int Socket::setSocketReuseAddr(int bOptVal)
{
    if(m_socket >= 0)
        return setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &bOptVal, sizeof(bOptVal));

    return -1;
}

int Socket::isSocketReuseAddr(void)
{
    socklen_t len;
    int optVal = -1;

    if(m_socket >= 0)
        getsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optVal, &len);
    else 
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return optVal;    
}

int Socket::setSocketKeepAlive(int bOptVal)
{
    if(m_socket >= 0)
        return setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &bOptVal, sizeof(bOptVal));
    else
        fprintf(stderr, "[File:%s][%s][Line:%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return -1;
}

bool Socket::isSocketKeepAlive(void)
{
    socklen_t len;
    int optVal = -1;

    if(m_socket >= 0)
        getsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, &optVal, &len);
    else
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    return optVal;    
}

int Socket::setSockOpt(int level, int optname, void *optval, socklen_t optlen)
{
    int ret;

    if((ret = setsockopt(m_socket, level, optname, optval, optlen)) < 0)
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return ret;
}

int Socket::read(void *buf, size_t size)
{
    int n = 0;

    if(m_socket < 0)
        return -1; 
again_:
    // It would read size to be 0 after socket closed.
    if((n = ::read(m_socket, (char *)buf, size)) < 0)
        if(errno == EAGAIN) 
            goto again_;

    return n;
}


int Socket::write(void *buf, size_t size)
{
    int n  = 0;

    if(m_socket < 0)
        return -1; 
again_:
    if((n = ::write(m_socket, (char *)buf, size)) < 0) 
        if(errno == EAGAIN) 
            goto again_; 

    return n;
}

void Socket::close(void)
{
    if(m_socket != -1) 
        ::close(m_socket);
    m_socket = -1;
}


