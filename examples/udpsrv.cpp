#include "../CUdpNetworking.h"

int main()
{
    struct sockaddr_storage cliAddr;
    socklen_t cliAddrLen = sizeof(cliAddr);
    char buf[1024] = {0};
    
    CUdpNetworking srv(55555);
    while(1) { 
        srv.Read(buf, sizeof(buf), (struct sockaddr *)&cliAddr, &cliAddrLen);
        printf("S <<< C:%s\n", buf);

        sprintf(buf,"%s", "Hello");
        srv.Write(buf, strlen(buf), (struct sockaddr *)&cliAddr, cliAddrLen);
    }
}
