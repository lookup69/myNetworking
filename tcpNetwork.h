/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef __LOOKUP69_TCP_NETWORKING_H__
#define __LOOKUP69_TCP_NETWORKING_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>

namespace lookup69 {
    typedef struct {
        int         family;
        uint16_t    port;
        std::string ip;
    } addressInfo_t;

    class TcpNetwork
    {
    private:
        int                     m_socket;
        int                     m_isServer;
        int                     m_domain;
        int                     m_port;
        bool                    m_isInitialized;
        std::string             m_host;
        struct sockaddr_storage m_addr;
        struct sockaddr_storage m_cliAddr;

    public:
        TcpNetwork(uint16_t port, const char *host = NULL, bool beServer = true, int domain = AF_INET);
        ~TcpNetwork();

        int initNetwork(bool bAutoLisentOrConnect = true);
        int getPeerName(addressInfo_t &addressInfo, int sd = -1);

        int Listen(int backlog = 50)
        {
            return listen(m_socket, backlog);
        }

        int Accept(struct sockaddr *addr = NULL, socklen_t *len = NULL);

        int Connect(const struct sockaddr *addr = NULL, socklen_t addrlen = 0)
        {
            if(addr == NULL)
                return connect(m_socket, (struct sockaddr *)&m_addr, sizeof(m_addr));

            return connect(m_socket, addr, addrlen);
        }

        int Read(void *buf, size_t size, int sd = -1);
        int Write(void *buf, size_t size, int sd = -1);

        int Close(int sd)
        {
            return close(sd);
        }
    };
}
#endif

