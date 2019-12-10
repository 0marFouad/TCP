#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include "Packet.h"
#include "ClientStarter.h"
#include <queue>

using namespace std;

// Driver code
int main() {
    int sip = 0;
    int port = 0;
    string filename;
    read_args(sip,port,filename);
    int sockfd = create_socket();
    initiate_connection(8080,"numbers.txt",sockfd);
    return 0;
}
