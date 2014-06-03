#include "../CTcpNetworking.h"
#include <stdio.h>

int main()
{
    CTcpNetworking cli(33333, "192.168.79.112", false);
    addressInfo_t  cliAddrInfo;
    char           buf[1024];

    if(cli.Connect() < 0){
        printf("connect fail\n");
        exit(0);
    }
    cli.getPeerName(cliAddrInfo);
    printf("connect success:%s(%d)\n", cliAddrInfo.ip.c_str(), cliAddrInfo.port);
    memset(buf, 0, sizeof(buf));
    sprintf(buf, "%s", "client send");
    cli.Write(buf, strlen(buf));

    memset(buf, 0, sizeof(buf));
    cli.Read(buf, sizeof(buf));
    printf("S >>> C:%s\n", buf);
}
