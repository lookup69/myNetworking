#ifndef _NETWORKING_SOCKET_H_
#define _NETWORKING_SOCKET_H_
namespace lookup69 {

class Socket 
{
protected:
    int m_sockfd;

private:
    Socket(const Socket &);
    Socket& operator=(const Socket&);
public:
    Socket(); 
    virtual ~Socket();

    int openSocket(int dommain, int type, int protocal = 0);
    int getSocket(void)
    {
        return m_sockfd;
    }

    bool isSocketOpen(void);
    int getSocketFamily(void);
    int setSocketReuseAddr(int bOptVal = 1);
    int isSocketReuseAddr(void);
    int setSocketKeepAlive(int bOptVal = 1);
    bool isSocketKeepAlive(void);
    int setSockOpt(int level, int optname, void *optval, socklen_t optlen);
};

} // namespace lookup69
#endif
