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
//#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <netinet/tcp.h>
#include <netdb.h>
#include <string.h>

#include <string>

namespace lookup69 {
    typedef struct {
        int         family;
        uint16_t    port;
        std::string ip;
    } addressInfo_t;

    class TcpSocket
    {
    private:
        int m_socket;

    private:
        TcpSocket(const TcpSocket &);
        TcpSocket &operator=(const TcpSocket &);
    public:
        TcpSocket(int sd);
        ~TcpSocket();

        int getAddrInfo(addressInfo_t &addressInfo);
        int read(void *buf, size_t size);
        int write(void *buf, size_t size);
        void close(void);
    };

    class TcpServer
    {
    private:
        int                     m_socket;

        TcpServer(const TcpServer &);
        TcpServer &operator=(const TcpServer &);
    public:
        TcpServer();
        virtual ~TcpServer();
        
        int serverInit(uint16_t port, int family = AF_UNSPEC, int backlog = 50, int protocol = 0);

        // need release(delete) connection 
        TcpSocket *getConnection(void);
    };


    class TcpClient
    {
    private:
        int         m_socket;

    public:
        TcpClient();
        virtual ~TcpClient();

        int clientInit(const std::string &host, uint16_t port, int family = AF_UNSPEC);
        TcpSocket* getConnection(void);
    };


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
            if(m_isServer || (addr == NULL))
                return -1;

            return connect(m_socket, addr, addrlen);
        }

        int Read(void *buf, size_t size, int sd = -1);
        int Write(void *buf, size_t size, int sd = -1);

        int Close(int sd = -1)
        {
            if(sd < 0) 
                return (m_socket >= 0) ? close(m_socket) : -1;
            return close(sd);
        }
    };
}
#endif

