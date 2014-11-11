/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#include "CTcpNetworking.h"
#include <errno.h>
#include <string.h>
#include <stdio.h>

CTcpNetworking::CTcpNetworking(uint16_t port, const char *host, bool beServer, int domain)
    : m_socket(-1), m_isServer(beServer), m_domain(domain), m_port(port)
{
    struct sockaddr_in  *pAddrIn = (struct sockaddr_in *) &m_addr;
    struct sockaddr_in6 *pAddrIn6 = (struct sockaddr_in6 *) &m_addr;
    int                 flagOn = 1;

    if((m_socket = socket(domain, SOCK_STREAM, 0)) < 0) {
        throw("CTcpNetworking::CTcpNetworking");
    }

    memset(&m_addr, 0, sizeof(m_addr));

    if(!beServer && (host == NULL)) {
        throw("CTcpNetworking::CTcpNetworking");
    }

    if(beServer) {
        if(setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&flagOn, sizeof(flagOn))){
            printf("[%s][%s][%d] setsockopt()....fail\n", __FILE__, __FUNCTION__, __LINE__);
            throw("CTcpNetworking::CTcpNetworking");
        }
    }

    if(domain == AF_INET) {
        pAddrIn->sin_family = AF_INET;
        pAddrIn->sin_addr.s_addr = (beServer) ? htonl(INADDR_ANY) : inet_addr(host);
        pAddrIn->sin_port = htons(port);
          
        if(beServer)
            if(bind(m_socket, (struct sockaddr *) pAddrIn, sizeof(struct sockaddr_in)) < 0) {
                printf("[%s][%s][%d] bind()....fail\n", __FILE__, __FUNCTION__, __LINE__);
                throw("CTcpNetworking::CTcpNetworking");
            }
    } else if(domain == AF_INET6) {
        const struct in6_addr in6addr_any = IN6ADDR_ANY_INIT;

        pAddrIn6->sin6_family = AF_INET6;
        if(beServer) 
            pAddrIn6->sin6_addr = in6addr_any;
        else
            inet_pton(AF_INET6, host, &(pAddrIn6->sin6_addr.s6_addr));
        pAddrIn6->sin6_port = htons(port);

        if(beServer) 
            if(bind(m_socket, (struct sockaddr *) pAddrIn6, sizeof(struct sockaddr_in6)) < 0) {
                    printf("[%s][%s][%d] bind()....fail\n", __FILE__, __FUNCTION__, __LINE__);
                    throw("CTcpNetworking::CTcpNetworking");
            }
    } else {
        throw("CTcpNetworking::CTcpNetworking");
    }
}

CTcpNetworking::~CTcpNetworking()     
{
    if(m_socket != -1)
        close(m_socket) ;
}

int CTcpNetworking::Listen(int backlog) 
{
    return listen(m_socket, backlog);
}

int CTcpNetworking::Connect(const struct sockaddr *addr, socklen_t addrlen)
{
    if(addr == NULL) { 
        return connect(m_socket, (struct sockaddr *)&m_addr, sizeof(m_addr));
    } else
        return connect(m_socket, addr, addrlen);

    return -1;
}

int CTcpNetworking::Accept(struct sockaddr *addr, socklen_t *len) 
{
    int ret = 0;
_again:
    if(addr == NULL) {
        socklen_t socklen = sizeof(m_cliAddr);;

        if((ret = accept(m_socket, (struct sockaddr *)&m_cliAddr, &socklen)) < 0)
            if(errno == EAGAIN) goto _again; 
    } else {
        if((ret = accept(m_socket, addr, len)) < 0)
            if(errno == EAGAIN) goto _again; 
    }

    return ret;
}

int CTcpNetworking::Read(void *buf, size_t size, int sd)
{
    int    sockfd = (sd == -1) ? m_socket : sd;
    int    readBytes = 0;
    size_t totalBytes = 0;
    
_again:
    do {
        // It would read size to be 0 after socket closed.
        if((readBytes = read(sockfd, ((char*)buf + totalBytes), size - (size_t)totalBytes)) <= 0) {
            if(errno == EAGAIN) 
                goto _again;
            else 
                break;
        }
        totalBytes += readBytes;
    } while(totalBytes < size);

    if(readBytes < 0)
        return readBytes; 

    return totalBytes;
}

int CTcpNetworking::Write(void *buf, size_t size, int sd)
{
    int    sockfd = (sd == -1) ? m_socket : sd;
    int    writeBytes = 0;
    size_t totalBytes = 0;

_again:
    do {
        if((writeBytes = write(sockfd, ((char *)buf + totalBytes), size - totalBytes)) < 0) {
            if(errno == EAGAIN) 
                goto _again; 
            else
                break;
        } 
        totalBytes += writeBytes;
    } while(totalBytes < size);

    if(writeBytes < 0)
        return writeBytes;
     
    return totalBytes;
}

int CTcpNetworking::getPeerName(addressInfo_t &addressInfo, int sd)
{
    struct sockaddr_storage name;
    socklen_t               socklen = sizeof(name);
    int                     ret;

    memset(&name, 0, sizeof(name));

    if(sd == -1) {
        if((ret = getpeername(m_socket, (struct sockaddr *)&name, &socklen)) < 0) {
            printf("getpeername() fail\n");
            return ret;
        } 
    } else {
        if((ret = getpeername(sd, (struct sockaddr *) &name, &socklen)) < 0) {
            printf("getpeername() fail\n");
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

int CTcpNetworking::Close(int sd)
{
    return close(sd);
}

