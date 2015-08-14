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
#include <string.h>
#include <stdio.h>

using namespace lookup69;

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


