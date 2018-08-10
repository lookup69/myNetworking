#include <iostream>

#include "../udpNetwork.h"

using namespace std;
using namespace lookup69;

static void print_usage(void)
{
    cout << "Usage:\n";
    cout << "Default port: 9966\n";
    cout << "\tudpcli -ip host [-port port]\n";
    exit(0);
}

int main(int argc, char *argv[])
{
    UdpClient udpClient;
    string    ip;
    int       port = 9966;
    
    if(argc >= 3) {
        for(auto i = 1; i < argc; ++i) {
            if(!string(argv[i]).compare("--help")) {
                print_usage();
            } else if(!string(argv[i]).compare("-ip")) {
                ++i;
                if(i < argc) 
                    ip = argv[i];
            } else if(!string(argv[i]).compare("-port")) {
                ++i;
                if(i < argc) 
                    port = atoi(argv[i]);
            }
        }
    }

    if(ip.empty() ) 
        print_usage();

    if(udpClient.clientInit(ip, port, AF_INET) < 0) {
        cout << "can not connect to " << ip << ":" << port << endl;
        exit(0);
    }
    cout << "connect to " << ip << ":" << port << endl;

    Socket *cli = udpClient.getConnection();
    if(!cli) {
        printf("getConnection() fail\n");
    }
    for(int i = 0; i < 1000; ++i) {
        char buf[1024] = {0};

        sprintf(buf, "cnt %d\n", i);
        printf("%s", buf);
        cli->write(buf, strlen(buf));
        memset(buf, 0, sizeof(buf));
        cli->read(buf, sizeof(buf));
        printf("%s", buf);
        sleep(1);
    }

    udpClient.releaseConnection(cli);

    return 0;
}
