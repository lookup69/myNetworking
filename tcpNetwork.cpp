/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#include "tcpNetwork.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
//#if __cplusplus >= 201103L
//#include <memory>
//#endif

using namespace lookup69;

/******************************************************************************
 * TcpSocket
 ******************************************************************************/
TcpSocket::TcpSocket(int sd) 
    : m_socket(sd)
{
}

TcpSocket::~TcpSocket()
{
    this->close();
}

int TcpSocket::getAddrInfo(addressInfo_t &addressInfo)
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

int TcpSocket::read(void *buf, size_t size)
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


int TcpSocket::write(void *buf, size_t size)
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

void TcpSocket::close(void)
{
    if(m_socket != -1) 
        ::close(m_socket);
    m_socket = -1;
}

/******************************************************************************
 * TcpServer
 ******************************************************************************/
TcpServer::TcpServer() : m_socket(-1)
{
}

TcpServer::~TcpServer()
{
    if(m_socket > 0) 
        ::close(m_socket);
}

int TcpServer::serverInit(uint16_t port, int family, int backlog, int protocol)
{
    int             optVal;
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char            _port[32] = {0};
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = family; 
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; /* Wildcard IP address; service name is numeric */

    sprintf(_port, "%d", port);
    if(getaddrinfo(NULL, _port, &hints, &result) != 0) {
        fprintf(stderr, "[File:%s][Func:%s][Line:%d] getaddrinfo() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    optVal = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        m_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (m_socket == -1)
            continue;

        if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optVal, sizeof(optVal)) < 0) 
            fprintf(stderr, "[File:%s][Func:%s][Line:%d] setSocketReuseAddr() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);

        if(bind(m_socket, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(m_socket);
        m_socket = -1;
    }
    freeaddrinfo(result);  

    if(m_socket < 0) {
        fprintf(stderr, "[File:%s][Func:%s][Line:%d] bind() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    if(listen(m_socket, backlog) < 0) {
        fprintf(stderr, "[File:%s][Func:%s][Line:%d] listen() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}

TcpSocket* TcpServer::getConnection(void)
{
    int             sd;
    TcpSocket       *tcpSocket;

    sd = accept(m_socket, NULL, 0);
    if(sd < 0){
        fprintf(stderr, "[File:%s][Func:%s][Line:%d] accpet() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return NULL;
    }

    tcpSocket = new(std::nothrow) TcpSocket(sd);

    return tcpSocket;
}

/******************************************************************************
 * TcpClient
 ******************************************************************************/
TcpClient::TcpClient() : m_socket(-1) 
{
}

TcpClient::~TcpClient()
{
    if(m_socket > 0) 
        ::close(m_socket);
}

int TcpClient::clientInit(const std::string &host, uint16_t port, int family)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    char            _port[32] = {0};
    
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = family;      /* Allows IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;

    sprintf(_port, "%d", port);
    printf("port:%s\n", _port);

    if (getaddrinfo(host.c_str(), _port, &hints, &result) != 0) {
        fprintf(stderr, "[File:%s][Func:%s][Line:%d] getaddrinfo() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        m_socket = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (m_socket == -1)
            continue;
        
        if (connect(m_socket, rp->ai_addr, rp->ai_addrlen) != -1)
            break; 
        close(m_socket);
        m_socket = -1;
    }
    freeaddrinfo(result);  

    if(m_socket == -1) 
        return -1;

    return 0;
}

TcpSocket* TcpClient::getConnection(void)
{
    if(m_socket < 0) 
        return NULL;
    return new(std::nothrow) TcpSocket(m_socket);
}

/******************************************************************************
 * TcpNetwork
 ******************************************************************************/
TcpNetwork::TcpNetwork(uint16_t port, const char *host, bool beServer, int domain)
    : m_socket(-1), m_isServer(beServer), m_domain(domain), m_port(port), m_isInitialized(false)
{
    if(host)
        m_host = host;
}

TcpNetwork::~TcpNetwork()     
{
    if(m_socket != -1)
        close(m_socket) ;
}

int TcpNetwork::initNetwork(bool bAutoLisentOrConnect)
{
    struct sockaddr_in  *pAddrIn = (struct sockaddr_in *) &m_addr;
    struct sockaddr_in6 *pAddrIn6 = (struct sockaddr_in6 *) &m_addr;
    int rtn = 0;

    if(m_isInitialized) 
        return 0;

    if(!m_isServer && (m_host.empty())) 
        return -1;

    if((m_socket = socket(m_domain, SOCK_STREAM, 0)) < 0)
        return -1;

    if(m_isServer) {
        int flagOn = 1;

        rtn = setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&flagOn, sizeof(flagOn));
    }

    // set address
    memset(&m_addr, 0, sizeof(m_addr));
    if(m_domain == AF_INET) {
        pAddrIn->sin_family = AF_INET;
        pAddrIn->sin_addr.s_addr = (m_isServer) ? htonl(INADDR_ANY) : inet_addr(m_host.c_str());
        pAddrIn->sin_port = htons(m_port);

        if((rtn == 0) && m_isServer) 
            rtn = bind(m_socket, (struct sockaddr *) pAddrIn, sizeof(struct sockaddr_in));

    } else if(m_domain == AF_INET6) {
        const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;

        pAddrIn6->sin6_family = AF_INET6;
        if(m_isServer) 
            pAddrIn6->sin6_addr = in6addr_any;
        else
            inet_pton(AF_INET6, m_host.c_str(), &(pAddrIn6->sin6_addr.s6_addr));
        pAddrIn6->sin6_port = htons(m_port);

        if((rtn == 0) && m_isServer) 
            rtn = bind(m_socket, (struct sockaddr *) pAddrIn6, sizeof(struct sockaddr_in6));
    } else {
        rtn = -1;
    }

    if((rtn == 0) && bAutoLisentOrConnect) {
        if(m_isServer) 
            rtn = Listen();
        else 
            rtn = Connect();
    }

    if(rtn == 0) {
        m_isInitialized = true;
    } else {
        close(m_socket);
        m_socket = -1;
        return -1;
    } 

    return 0;
}



int TcpNetwork::Accept(struct sockaddr *addr, socklen_t *len) 
{
    int ret = 0;

again_:
    if((ret = accept(m_socket, addr, len)) < 0)
        if(errno == EAGAIN) 
            goto again_; 

    return ret;
}

int TcpNetwork::Read(void *buf, size_t size, int sd)
{
    int sockfd = (sd == -1) ? m_socket : sd;
    int n = 0;
    
    // The server can't use the default socket to read
    if(m_isServer && (sd == -1)) 
        return -1;

again_:
    // It would read size to be 0 after socket closed.
    if((n = read(sockfd, (char *)buf, size)) < 0)
        if(errno == EAGAIN) 
            goto again_;

    return n;
}

int TcpNetwork::Write(void *buf, size_t size, int sd)
{
    int    sockfd = (sd == -1) ? m_socket : sd;
    int    n = 0;

    // The server can't use the default socket to write.
    if(m_isServer && (sd == -1)) 
        return -1;
again_:
    if((n = write(sockfd, (char *)buf, size)) < 0) 
        if(errno == EAGAIN) 
            goto again_; 
     
    return n;
}

int TcpNetwork::getPeerName(addressInfo_t &addressInfo, int sd)
{
    struct sockaddr_storage name;
    socklen_t               socklen = sizeof(name);
    int                     ret;

    memset(&name, 0, sizeof(name));
    if(sd == -1) {
        if((ret = getpeername(m_socket, (struct sockaddr *)&name, &socklen)) < 0) {
            return ret;
        } 
    } else {
        if((ret = getpeername(sd, (struct sockaddr *) &name, &socklen)) < 0) {
            return ret;
        } 
    }

    addressInfo.family = ((struct sockaddr *)&name)->sa_family;

    if(addressInfo.family == AF_INET) {
        struct sockaddr_in *addrIn = (struct sockaddr_in *) &name;

        addressInfo.port = ntohs(addrIn->sin_port);
        addressInfo.ip = inet_ntoa(addrIn->sin_addr);
    } else if(addressInfo.family == AF_INET6) {
        struct sockaddr_in6 *addrIn6 = (struct sockaddr_in6 *) &name;
        char ipTmp[512] = {0};

        addressInfo.port = ntohs(addrIn6->sin6_port);
        inet_ntop(AF_INET6, &addrIn6->sin6_addr, ipTmp, sizeof(ipTmp));
        addressInfo.ip = ipTmp;
    } else 
        ret = -1;

    return ret;
}


