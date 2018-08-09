#ifndef _NETWORKING_SOCKET_H_
#define _NETWORKING_SOCKET_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

namespace lookup69 {

    typedef struct {
        int         family;
        uint16_t    port;
        std::string ip;
    } addressInfo_t;

    class Socket 
    {
    protected:
        int m_socket;

    private:
        Socket(const Socket &);
        Socket& operator=(const Socket&);
    public:
        Socket(int sd = -1);
        virtual ~Socket();

        bool isSocketOpen(void);
        int getAddrInfo(addressInfo_t &addressInfo);
        int setSocketReuseAddr(int bOptVal = 1);
        int isSocketReuseAddr(void);
        int setSocketKeepAlive(int bOptVal = 1);
        bool isSocketKeepAlive(void);
        int setSockOpt(int level, int optname, void *optval, socklen_t optlen);

        virtual int read(void *buf, size_t size);
        virtual int write(void *buf, size_t size);
        virtual void close(void);
};

} // namespace lookup69
#endif
