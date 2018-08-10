//#include <string.h>

#include <iostream>
#include "../udpNetwork.h"

using namespace std;
using namespace lookup69;

static void print_usage(void)
{
    cout << "Usage:\n";
    cout << "Default port: 9966\n";
    cout << "\tudpsrv [-port port]\n";
    exit(0);
}

int main(int argc, char *argv[])
{
    UdpServer udpServer;
    Socket    *s;
    int       port = 9966;
    
    if(argc > 2) {
        for(auto i = 1; i < argc; ++i) {
            if(!string(argv[i]).compare("--help")) 
                print_usage();
            if(!string(argv[i]).compare("-port")) {
                ++i;
                if(i < argc) {
                    port = atoi(argv[i]);
                    break;
                }
            }
        }
    }

    cout << "PID:" << getpid() << "    port:" << port << endl;
    if(udpServer.serverInit(port, AF_INET) < 0) {
        printf("serverInit fail\n");
        exit(0);
    }

    s = udpServer.getConnection();
    if(s == NULL) {
        cout << "udpServer.getConnection() ... fail\n";
        return 0;
    }

    while(1) {
        addressInfo_t addrInfo;
        char          rbuf[1024] = {0};
        char          wbuf[1024] = {0};

        if(s->read(rbuf, sizeof(rbuf)) <= 0)
            break;
        s->getAddrInfo(addrInfo);    
        cout << "C(" << addrInfo.ip << ":" << addrInfo.port << ") ->S(" << getpid() << "):" << rbuf;
        sprintf(wbuf, "S(%d)->C: %s\n", getpid(), rbuf);
        if(s->write(wbuf, strlen(wbuf)) < 0)
            break;
        
        udpServer.releaseConnection(s);
    }
    return 0;
}
