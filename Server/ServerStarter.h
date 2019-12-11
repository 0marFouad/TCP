#ifndef SERVER_SERVERSTARTER_H
#define SERVER_SERVERSTARTER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include "Server.h"
#include "Packet.h"
#include "Server.h"

using namespace std;

int create_socket(int &port);
void accept(int sockfd, int seed, double plp);

#endif
