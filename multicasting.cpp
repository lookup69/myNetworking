#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "multicasting.h"

using namespace std;

CMulticasting::CMulticasting(const char *maaddr, unsigned short port, int family) : 
    CUdp(family, port)
{
    m_multicastAddr = maaddr;
}


int CMulticasting::setMulticastInterface(const char *interface)
{
    switch(getSocketFamily()) {
    case AF_INET: // IPv4
        {
            struct ifreq    ifr;
            struct in_addr  inaddr;

            memset(&ifr, 0, sizeof(ifr));
            memset(&inaddr, 0, sizeof(inaddr));

            sprintf(ifr.ifr_name, "%s", interface);
            if(ioctl(m_sockfd, SIOCGIFADDR, &ifr) < 0) {
                fprintf(stderr, "[File:%s][%s][Line:%d] ioctl() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
                return -1;
            }

            memcpy(&inaddr, &((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr, sizeof(inaddr));
            return setsockopt(m_sockfd, IPPROTO_IP, IP_MULTICAST_IF, &inaddr, sizeof(inaddr));
        }
        break;

    default:
        return -1;
    }

    return -1;
}

int CMulticasting::joinByInterfaceIndex(int interfaceIndex)
{
    struct ip_mreqn mreqn;
    //short  flags;

    memset(&mreqn, 0, sizeof(mreqn));
    mreqn.imr_multiaddr.s_addr = inet_addr(m_multicastAddr.c_str());
    mreqn.imr_ifindex = interfaceIndex;


    if(setsockopt(m_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreqn, sizeof(mreqn)) < 0) {
        fprintf(stderr, "[File:%s][%s][Line:%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
    }

    return 0;
}


#if 0
int CMulticasting::joinByInterfaceAddr(const char *ip)
{
    struct ip_mreq mreq;

    memset(&mreq, 0, sizeof(mreq));
    mreq.imr_multiaddr.s_addr = inet_addr(m_multicastAddr.c_str());

    if(ip)
        inet_aton(ip, &mreq.imr_interface.s_addr);
    else
        mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if(setsockopt(m_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        fprintf(stderr, "[File:%s][%s][Line:%d] setsockopt() ... fail\n", __FILE__, __PRETTY_FUNCTION__, __LINE__);
        return -1;
    }

    return 0;
}
#endif
