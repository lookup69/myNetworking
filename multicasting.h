#ifndef _CMULTICASTING_H_
#define _CMULTICASTING_H_
#include <string>
#include "udp.h"

class CMulticasting : public CUdp
{
private:
    std::string m_multicastAddr;

public:
    
    CMulticasting(const char *maaddr, unsigned short port, int family = AF_INET);
    ~CMulticasting() {};

    void setMulticastAddr(std::string maddr)
    {
        m_multicastAddr = maddr;
    }
    
    int setMulticastInterface(const char *interface);
    /*
        if interfaceIndex equal to zero that
        it means to indicate any interface
    */
    int joinByInterfaceIndex(int interfaceIndex = 0);

    /*
        If there is no assigned ip address then the                   .
        system will use INADDR_ANY to intend of ip address
                                                           .
        Let the kenel to decide which interface will been joined.
    */
    //int joinByInterfaceAddr(const char *ip = NULL);

};

#endif
