#ifndef _NETWORKING_SOCKET_H_
#define _NETWORKING_SOCKET_H_

class CSocket 
{
protected:
    int m_sockfd;

public:
    CSocket() : m_sockfd(-1) {};
    CSocket(int dommain, int type, int protocal = 0);
    ~CSocket();

    int getSocketFd(void)
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
#endif
