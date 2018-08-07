#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"

using namespace lookup69;

Socket::Socket() : m_sockfd(-1) 
{
}

Socket::~Socket(void)
{
    if(m_sockfd >= 0)
        close(m_sockfd);
}

int Socket::openSocket(int dommain, int type, int protocol)
{
    m_sockfd = socket(dommain, type, protocol);
    if(m_sockfd < 0)
        fprintf(stderr, "[File:%s][Func:%s][Line:%d] socket() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return m_sockfd;
}

bool Socket::isSocketOpen(void)
{
    if(m_sockfd >= 0)
        return true;

    return false;
}

int Socket::getSocketFamily(void)
{
    struct sockaddr sa;
    socklen_t len = sizeof(sa);

    if (getsockname(m_sockfd, &sa, &len) < 0) {
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockname() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    return sa.sa_family;
}


int Socket::setSocketReuseAddr(int bOptVal)
{
    if(m_sockfd >= 0)
        return setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &bOptVal, sizeof(bOptVal));

    return -1;
}

int Socket::isSocketReuseAddr(void)
{
    socklen_t len;
    int optVal = -1;

    if(m_sockfd >= 0)
        getsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, &len);
    else 
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return optVal;    
}

int Socket::setSocketKeepAlive(int bOptVal)
{
    if(m_sockfd >= 0)
        return setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &bOptVal, sizeof(bOptVal));
    else
        fprintf(stderr, "[File:%s][%s][Line:%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return -1;
}

bool Socket::isSocketKeepAlive(void)
{
    socklen_t len;
    int optVal = -1;

    if(m_sockfd >= 0)
        getsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optVal, &len);
    else
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    return optVal;    
}

int Socket::setSockOpt(int level, int optname, void *optval, socklen_t optlen)
{
    int ret;

    if((ret = setsockopt(m_sockfd, level, optname, optval, optlen)) < 0)
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return ret;
}



