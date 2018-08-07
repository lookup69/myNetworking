//#include <string.h>

#include <iostream>
#include "../tcpNetwork.h"

using namespace std;
using namespace lookup69;

static void print_usage(void)
{
    cout << "Usage:\n";
    cout << "Default port: 9966\n";
    cout << "\ttcpSrv2 [-port port]\n";
    exit(0);
}

int main(int argc, char *argv[])
{
    TcpServer tcpServer;
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
    if(tcpServer.serverInit(port, AF_INET) < 0) {
        printf("serverInit fail\n");
        exit(0);
    }

    while(1) {
        addressInfo_t addrInfo;
        pid_t         pid;

        TcpSocket *s = tcpServer.getConnection();
        
        if(s) {
            s->getAddrInfo(addrInfo);    
            pid = fork();
            if(pid == 0) {
                while(1) {
                    char rbuf[1024] = {0};
                    char wbuf[1024] = {0};
                    
                    if(s->read(rbuf, sizeof(rbuf)) <= 0)
                        break;
                    cout << "C(" << addrInfo.ip << ":" << addrInfo.port << ") ->S(" << getpid() << "):" << rbuf;
                    sprintf(wbuf, "S(%d)->C: %s\n", getpid(), rbuf);
                    if(s->write(wbuf, strlen(wbuf)) < 0)
                        break;
                }
                delete s;
                exit(0);
            } 
            delete s;
        }
    }
    return 0;
}
