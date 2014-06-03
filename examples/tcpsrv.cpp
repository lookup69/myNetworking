#include "../CTcpNetworking.h"
#include <stdio.h>

int main()
{
    try {
        CTcpNetworking  srv(33333);
        addressInfo_t   cliAddrInfo;
        int             clisd;
        char            buf[1024];

        if(srv.Listen() < 0) {
            printf("listen() fail\n");
            exit(0);
        }
        while ((clisd = srv.Accept()) != -1) {
            srv.getPeerName(cliAddrInfo, clisd);
            printf("connect from:%s(%d)\n", cliAddrInfo.ip.c_str(), cliAddrInfo.port);

            memset(buf, 0, sizeof(buf));
            srv.Read(buf, sizeof(buf), clisd);
            printf("C >>> S:%s\n", buf);
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%s", "Server echo");
            srv.Write(buf, strlen(buf), clisd);
            close(clisd);
        }
    }
    catch(const char *e) {
        printf("%s\n", e);
    }
}
