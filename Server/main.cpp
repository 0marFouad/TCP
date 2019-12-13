#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "ServerStarter.h"
#include <vector>

// Driver code
int main() {
    int port = 8080;
    int seed = 90;
    double plp = 0.15;
    srand(seed);
    read_args(port,seed,plp);
    int sockfd = create_socket(port);
    accept(sockfd,seed,plp);
    return 0;
}
