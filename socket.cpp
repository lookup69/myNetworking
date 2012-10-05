#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "socket.h"


CSocket::CSocket(int dommain, int type, int protocal)
{
    m_sockfd = socket(dommain, type, protocal);
    if(m_sockfd < 0)
        fprintf(stderr, "[File:%s][%s][Line:%d] socket() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
}

CSocket::~CSocket(void)
{
    if(m_sockfd >= 0)
        close(m_sockfd);
}

bool CSocket::isSocketOpen(void)
{
    if(m_sockfd >= 0)
        return true;

    return false;
}

int CSocket::getSocketFamily(void)
{
    struct sockaddr sa;
    socklen_t len = sizeof(sa);

    if (getsockname(m_sockfd, &sa, &len) < 0) {
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockname() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    return sa.sa_family;
}


int CSocket::setSocketReuseAddr(int bOptVal)
{
    if(m_sockfd >= 0)
        return setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &bOptVal, sizeof(bOptVal));

    return -1;
}

int CSocket::isSocketReuseAddr(void)
{
    socklen_t len;
    int optVal = -1;

    if(m_sockfd >= 0)
        getsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &optVal, &len);
    else 
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return optVal;    
}

int CSocket::setSocketKeepAlive(int bOptVal)
{
    if(m_sockfd >= 0)
        return setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &bOptVal, sizeof(bOptVal));
    else
        fprintf(stderr, "[File:%s][%s][Line:%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return -1;
}

bool CSocket::isSocketKeepAlive(void)
{
    socklen_t len;
    int optVal = -1;

    if(m_sockfd >= 0)
        getsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &optVal, &len);
    else
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    return optVal;    
}

int CSocket::setSockOpt(int level, int optname, void *optval, socklen_t optlen)
{
    int ret;

    if((ret = setsockopt(m_sockfd, level, optname, optval, optlen)) < 0)
        fprintf(stderr, "[File:%s][%s][Line:%d] getsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

    return ret;
}



