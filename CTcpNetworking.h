/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef __CTCP_NETWORKING_H__
#define __CTCP_NETWORKING_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

typedef struct {
    int         family;
    uint16_t    port;
    std::string ip;
} addressInfo_t;

class CTcpNetworking 
{
private:
    int                     m_socket;
    int                     m_isServer;
    int                     m_domain;
    int                     m_port;
    std::string             m_host;
    struct sockaddr_storage m_addr;
    struct sockaddr_storage m_cliAddr;

public:
    CTcpNetworking(uint16_t port, const char *host = NULL, bool beServer = true, int domain = AF_INET);
    virtual ~CTcpNetworking();

    int getPeerName(addressInfo_t &addressInfo, int sd = -1);

    virtual int Listen(int backlog = 50);
    virtual int Accept(struct sockaddr *addr=NULL, socklen_t *len=NULL);
    virtual int Connect(const struct sockaddr *addr = NULL, socklen_t addrlen = 0);
    virtual int Read(void *buf, size_t size, int sd = -1);
    virtual int Write(void *buf, size_t size, int sd = -1);
};
#endif

