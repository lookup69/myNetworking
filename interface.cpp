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

#include "interface.h"

using namespace std;

#ifdef CNETINTERFACE_DEBUG
void CNetInterface::flagDump(const string &name, short index, short flags) const
{
    printf("if:%s  index:%d  flags:", name.c_str(), index);
    if(flags & IFF_UP)
        printf("IFF_UP ");
    if(flags & IFF_BROADCAST)
        printf("IFF_BROADCAST ");
    if(flags & IFF_DEBUG)
        printf("IFF_DEBUG ");
    if(flags & IFF_LOOPBACK)
        printf("IFF_LOOPBACK ");
    if(flags & IFF_POINTOPOINT)
        printf("IFF_POINTOPOINT ");
    if(flags & IFF_RUNNING)
        printf("IFF_RUNNING ");
    if(flags & IFF_NOARP)
        printf("IFF_NOARP ");
    if(flags & IFF_PROMISC)
        printf("IFF_PROMISC ");
    if(flags & IFF_NOTRAILERS)
        printf("IFF_NOTRAILERS ");
    if(flags & IFF_ALLMULTI)
        printf("IFF_ALLMULTI ");
    if(flags & IFF_MASTER)
        printf("IFF_MASTER ");
    if(flags & IFF_SLAVE)
        printf("IFF_SLAVE ");
    if(flags & IFF_MULTICAST)
        printf("IFF_MULTICAST ");
    if(flags & IFF_PORTSEL)
        printf("IFF_PORTSEL ");
    if(flags & IFF_AUTOMEDIA)
        printf("IFF_AUTOMEDIA ");
    if(flags & IFF_DYNAMIC)
        printf("IFF_DYNAMIC ");
    printf("\n");
}
#endif

CNetInterface::CNetInterface()
{
    initialInterfaceInfo();
}

int CNetInterface::initialInterfaceInfo(void)
{
    struct ifconf ifc;
    struct ifreq  *pIfr;
    char *ifcBuf = NULL;
    size_t sizeOfIfr;
    size_t i, len;
    int sockfd, lastlen;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        return -1;

    lastlen = 0;
    len = 100 * sizeof(struct ifreq); /* initial buffer size guess */
    for( ; ; ) {
        ifcBuf = (char *) malloc(len);
        ifc.ifc_len = len;
        ifc.ifc_buf = ifcBuf;
        if(ioctl(sockfd, SIOCGIFCONF, &ifc) < 0) {
            if(errno != EINVAL || lastlen != 0) {
                fprintf(stderr, "[%s][Line:%d] ioctl() ... fail\n", __PRETTY_FUNCTION__, __LINE__);
                if(ifcBuf)
                    free(ifcBuf);

                close(sockfd);
                return -1;
            }
        } else {
            if(ifc.ifc_len == lastlen)
                break;    /* success, len has not changed */
            lastlen = ifc.ifc_len;
        }
        len += 10 * sizeof(struct ifreq); /* increment */
        free(ifcBuf);
    }


    sizeOfIfr = ifc.ifc_len / sizeof(*pIfr);
    pIfr =  ifc.ifc_ifcu.ifcu_req;

    // clear all data in the m_ifiInfoVec and prepare
    // to insert new data
    m_ifiInfoVec.clear();
    for(i = 0; i < sizeOfIfr; i++) {
        struct ifiInfo ifi;

        memset(&ifi.hwaddr, 0, sizeof(ifi.hwaddr));
        memset(&ifi.brdaddr, 0, sizeof(ifi.brdaddr));
        ifi.flags = 0;
        ifi.index = 0;
        ifi.mtu = 0;
        ifi.name = (pIfr + i)->ifr_name;
        ifi.addr = (pIfr + i)->ifr_addr;

        m_ifiInfoVec.push_back(ifi);
    }

    free(ifcBuf);


    for(i = 0; i < m_ifiInfoVec.size(); i++) {
        struct ifreq ifr;

        memset(&ifr, 0, sizeof(ifr));
        sprintf(ifr.ifr_name, "%s", m_ifiInfoVec[i].name.c_str());

        // get index
        if(ioctl(sockfd, SIOCGIFINDEX, &ifr) < 0) {
            if(errno != EINVAL || lastlen != 0) {
                fprintf(stderr, "[%s][Line:%d] ioctl() ... fail\n", __PRETTY_FUNCTION__, __LINE__);
            }
        } else {
            m_ifiInfoVec[i].index = ifr.ifr_ifindex;
        }

        // get interface mtu
        if(ioctl(sockfd, SIOCGIFMTU, &ifr) < 0) {
            if(errno != EINVAL || lastlen != 0) {
                fprintf(stderr, "[%s][Line:%d] ioctl() ... fail\n", __PRETTY_FUNCTION__, __LINE__);
            }
        } else {
            m_ifiInfoVec[i].mtu = ifr.ifr_mtu;
        }

        // get interface flags
        if(ioctl(sockfd, SIOCGIFFLAGS, &ifr) < 0) {
            if(errno != EINVAL || lastlen != 0) {
                fprintf(stderr, "[%s][Line:%d] ioctl() ... fail\n", __PRETTY_FUNCTION__, __LINE__);
            }
        } else {
            m_ifiInfoVec[i].flags = ifr.ifr_flags;
        }

        // get interface hardware address
        if(ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
            if(errno != EINVAL || lastlen != 0) {
                fprintf(stderr, "[%s][Line:%d] ioctl() ... fail\n", __PRETTY_FUNCTION__, __LINE__);
            }
        } else {
            m_ifiInfoVec[i].hwaddr = ifr.ifr_hwaddr;
        }

        sprintf(ifr.ifr_name, "%s", m_ifiInfoVec[i].name.c_str());

        // get broadcast address
        if(ioctl(sockfd, SIOCGIFBRDADDR, &ifr) < 0) {
            if(errno != EINVAL || lastlen != 0) {
                fprintf(stderr, "[%s][Line:%d] ioctl() ... fail\n", __PRETTY_FUNCTION__, __LINE__);
            }
        } else {
            m_ifiInfoVec[i].brdaddr = ifr.ifr_broadaddr;
        }

    }

    close(sockfd);
    return 0;
}


#ifdef CNETINTERFACE_TEST
int main()
{
    CNetInterface ethIf;
    unsigned int numOfInterface;
    unsigned int i;

    numOfInterface = ethIf.getNumOfInterface();

    for(i = 0; i < numOfInterface; i++) {
        const struct ifiInfo *pIfi = NULL;

        pIfi = ethIf.getInterfaceInfo(i);
        ethIf.flagDump(pIfi->name.c_str(), pIfi->index, pIfi->flags);
    }

    return 0;
}

#endif
