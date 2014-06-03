/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#include "CUdpNetworking.h"
#include <errno.h>
#include <string.h>

CUdpNetworking::CUdpNetworking(int domain)
{
    if((m_socket = socket(domain, SOCK_DGRAM, 0)) < 0) {
        throw("CTcpNetworking::CTcpNetworking");
    }
}

CUdpNetworking::CUdpNetworking(uint16_t port, const char *host, bool beServer, int domain)
    : m_socket(-1), m_isServer(beServer), m_isConnect(false), m_domain(domain), m_port(port)
{
    struct sockaddr_in  *pAddrIn = (struct sockaddr_in *) &m_selfAddr;
    struct sockaddr_in6 *pAddrIn6 = (struct sockaddr_in6 *) &m_selfAddr;

    if((m_socket = socket(domain, SOCK_DGRAM, 0)) < 0) {
        throw("CTcpNetworking::CTcpNetworking");
    }

    memset(&m_selfAddr, 0, sizeof(m_selfAddr));
    memset(&m_dstAddr, 0, sizeof(m_dstAddr));

    if(domain == AF_INET) {
        pAddrIn->sin_family = AF_INET;
        pAddrIn->sin_addr.s_addr = (host == NULL) ? htonl(INADDR_ANY) : inet_addr(host);
        pAddrIn->sin_port = (beServer) ? htons(port) : 0;
        
        if(bind(m_socket, (struct sockaddr *) pAddrIn, sizeof(struct sockaddr_in)) < 0) {
            printf("[%s][%s][%d] bind()....fail\n", __FILE__, __FUNCTION__, __LINE__);
            throw("CTcpNetworking::CTcpNetworking");
        }
        if(!beServer) {
            memcpy(&m_dstAddr, &m_selfAddr, sizeof(m_dstAddr));
            ((struct sockaddr_in *)&m_dstAddr)->sin_port = htons(port); 
        }
    } else if(domain == AF_INET6) {

        const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;

        pAddrIn6->sin6_family = AF_INET6;
        if(host == NULL) 
            pAddrIn6->sin6_addr = in6addr_any;
        else
            inet_pton(AF_INET6, host, &(pAddrIn6->sin6_addr.s6_addr));
        pAddrIn6->sin6_port = (beServer) ? htons(port) : 0;

        if(bind(m_socket, (struct sockaddr *) pAddrIn6, sizeof(struct sockaddr_in6)) < 0) {
            printf("[%s][%s][%d] bind()....fail\n", __FILE__, __FUNCTION__, __LINE__);
            throw("CTcpNetworking::CTcpNetworking");
        }
        if(!beServer) {
            memcpy(&m_dstAddr, &m_selfAddr, sizeof(m_dstAddr));
            ((struct sockaddr_in6 *)&m_dstAddr)->sin6_port = htons(port); 
        }
    } else {
        throw("CTcpNetworking::CTcpNetworking");
    }
}

CUdpNetworking::~CUdpNetworking()
{
    if(m_socket != -1)
        close(m_socket) ;
}

int CUdpNetworking::Connect(const struct sockaddr *addr, socklen_t addrlen)
{
    int ret = 0;

    if(addr == NULL) { 
        ret = connect(m_socket, (struct sockaddr *)&m_dstAddr, sizeof(m_dstAddr));
    } else
        ret = connect(m_socket, addr, addrlen);

    if(ret == 0)
        m_isConnect = true;
     
    return ret;
}

int CUdpNetworking::Read(void *buf, size_t size, struct sockaddr *srcAddr, socklen_t *addrlen)
{
    int ret;
    
_again:
    if(m_isConnect) {
        if((ret = read(m_socket, buf, size)) < 0) {
            if(errno == EINTR) goto _again;
        }
    } else {
        if((ret = recvfrom(m_socket, buf, size, 0, srcAddr, addrlen)) < 0) {
            if(errno == EINTR) goto _again; 
        }
    }
    
    return ret;
}

int CUdpNetworking::Write(void *buf, size_t size, struct sockaddr *dstAddr, socklen_t addrlen)
{
    int ret;

_again:
    if(m_isConnect) {
        if((ret = write(m_socket, buf, size)) < 0) {
            if(errno == EINTR) goto _again; 
        }
    } else {
        if((ret = sendto(m_socket, buf, size, 0, dstAddr, addrlen)) < 0) {
            if(errno == EINTR) goto _again; 
        }
    }

    return ret;
}

void CUdpNetworking::addressStuff(struct sockaddr *addr, const char *host, uint16_t port, int domain)
{
    if(domain == AF_INET) {
        struct sockaddr_in  *pAddrIn = (struct sockaddr_in *) addr;

        pAddrIn->sin_family = AF_INET;
        pAddrIn->sin_addr.s_addr = inet_addr(host);
        pAddrIn->sin_port = htons(port);
    } else if(domain == AF_INET6) {
        struct sockaddr_in6 *pAddrIn6 = (struct sockaddr_in6 *) addr;

        pAddrIn6->sin6_family == AF_INET6;
        inet_pton(AF_INET6, host, &(pAddrIn6->sin6_addr.s6_addr));
        pAddrIn6->sin6_port = htons(port);
    }
}



