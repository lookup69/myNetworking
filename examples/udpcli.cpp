#include "../CUdpNetworking.h"

int main()
{
    char buf[1024] = {0};

// Use case 1
#if 0
    CUdpNetworking cli(55555, "192.168.79.112", false);

    cli.Connect();
    sprintf(buf, "%s", "Hello");
    cli.Write(buf, strlen(buf));

    memset(buf, 0, sizeof(buf));
    cli.Read(buf, sizeof(buf));
    printf("S >>> C: %s\n", buf);

// Use case 2
#else
    CUdpNetworking cli;
    struct sockaddr_storage srvAddr;
    socklen_t srvAddrLen = sizeof(srvAddr);

    CUdpNetworking::addressStuff((struct sockaddr*) &srvAddr, "192.168.79.112", 55555);

    sprintf(buf, "%s", "Hello");
    cli.Write(buf, strlen(buf),(struct sockaddr*) &srvAddr, sizeof(srvAddr));

    memset(buf, 0, sizeof(buf));
    cli.Read(buf, sizeof(buf), (struct sockaddr*) &srvAddr, &srvAddrLen);
    printf("S >>> C: %s\n", buf);

#endif
}
