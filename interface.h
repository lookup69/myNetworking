#ifndef _CNETINTERFACE_H_
#define _CNETINTERFACE_H_
#include <sys/socket.h>
#include <vector>
#include <string>

#define CNETINTERFACE_DEBUG
//#define CNETINTERFACE_TEST

struct ifiInfo {
    std::string     name;       /* interface name, null-terminated */
    struct sockaddr addr;       /* primary address */
    struct sockaddr brdaddr;    /* broadcast address */
    struct sockaddr hwaddr;     /* hardware address */
    short           index;      /* interface index */
    short           mtu;        /* interface MTU */
    short           flags;      /* IFF_xxx constants from <net/if.h> */
};



class CNetInterface {
private:
    std::vector<ifiInfo> m_ifiInfoVec;

public:
    CNetInterface();

    int initialInterfaceInfo(void);

    unsigned int getNumOfInterface(void)
    {
        return m_ifiInfoVec.size();
    }

    const char *getInterfaceName(unsigned int index) const
    {
        if(index >= m_ifiInfoVec.size())
            return NULL;

        return m_ifiInfoVec[index].name.c_str();
    }

    short getInterfaceFlags(unsigned int index) const
    {
        if(index >= m_ifiInfoVec.size())
            return 0;
        
        return m_ifiInfoVec[index].flags;
    }

    short getInterfaceIndex(unsigned int index) const
    {
        if(index >= m_ifiInfoVec.size())
            return 0;

        return m_ifiInfoVec[index].index;
    }

    const struct ifiInfo *getInterfaceInfo(unsigned int index) const
    {
        if(index >= m_ifiInfoVec.size())
            return NULL;

        return &m_ifiInfoVec[index];
    }

#ifdef CNETINTERFACE_DEBUG
    void flagDump(const std::string &name, short index, short flags) const;
#endif
};


#endif //_CINTERFACE_H_