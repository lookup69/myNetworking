/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef __CUDP_NETWORKING_H__
#define __CUDP_NETWORKING_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string>

class CUdpNetworking 
{
private:
    int                     m_socket;
    int                     m_isServer;
    int                     m_isConnect;
    int                     m_domain;
    int                     m_port;
    std::string             m_host;
    struct sockaddr_storage m_selfAddr;
    struct sockaddr_storage m_dstAddr;

public:
    CUdpNetworking(int domain = AF_INET);
    CUdpNetworking(uint16_t port, const char *host = NULL, bool beServer = true, int domain = AF_INET);
    virtual ~CUdpNetworking();

    virtual int Connect(const struct sockaddr *addr = NULL, socklen_t addrlen = 0);
    int Read(void *buf, size_t size, struct sockaddr *srcAddr = NULL, socklen_t *addrlen = NULL);
    int Write(void *buf, size_t size, struct sockaddr *dstAddr = NULL, socklen_t addrlen = 0);

    static void addressStuff(struct sockaddr *addr, const char *host, uint16_t port, int domain = AF_INET);
};

#endif
