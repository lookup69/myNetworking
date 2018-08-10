/*
 * @brief   
 *
 * @author  Archer Chang
 * @file    
 * @date    
 *
 */

#ifndef __LOOKUP69_TCP_NETWORK_H__
#define __LOOKUP69_TCP_NETWORK_H__


#include <unistd.h>
//#include <netinet/tcp.h>
#include <netdb.h>
#include <string.h>

#include <string>

#include "socket.h"

namespace lookup69 {
    class TcpSocket : public Socket
    {
    private:
        // Hide interface
        TcpSocket(const TcpSocket &);
        TcpSocket &operator=(const TcpSocket &);

    public:
        TcpSocket(int sd = -1);
        virtual ~TcpSocket();

        int bind(uint16_t port, int family = AF_UNSPEC);
        int listen(int backlog = 50);
        int accpet(void);
        int connect(const std::string &host, uint16_t port, int family = AF_UNSPEC);
        Socket *getConnection(void);
    };

    class TcpServer
    {
    private:
        TcpSocket m_tcpSocket;

        // Hide interface
        TcpServer(const TcpServer &);
        TcpServer &operator=(const TcpServer &);

    public:
        TcpServer();
        virtual ~TcpServer();
        
        int serverInit(uint16_t port, int family = AF_UNSPEC, int backlog = 50);

        Socket *getConnection(void);
        void releaseConnection(Socket *socket);
    };


    class TcpClient
    {
    private:
        TcpSocket m_tcpSocket;

    public:
        TcpClient();
        virtual ~TcpClient();

        int clientInit(const std::string &host, uint16_t port, int family = AF_UNSPEC);
        Socket* getConnection(void);
        void releaseConnection(Socket *socket);
    };

#if 0
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
#endif
}
#endif

